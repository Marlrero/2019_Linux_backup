#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

void error_handling(char* message);
void read_childproc(int sig);
void child_proc_file(int fds);
void child_proc_sock(int sock, int fds, char* buf);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;

	char buf[BUF_SIZE];

	int fds[2];

	pid_t pid;
	struct sigaction act;
	int state;
	
	if (argc != 2) {
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	// signal action
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	// socket
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // tcp
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error!");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	// parent, child process -> pipe
	pipe(fds);
	pid = fork();
	if (pid == 0) { // child
		child_proc_file(fds[0]); // out -> 수신
		return 0; // child process return 0
	}
	
	// parent
	while (1) {
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if (clnt_sock == -1) // accept error
			continue;
		else
			puts("new client connected...");

		pid = fork();
		if (pid == 0) { // child 
			close(serv_sock);
			child_proc_sock(clnt_sock, fds[1], buf); // in -> 송신
			return 0;
		} else // parent
			close(clnt_sock);
	}
	
	close(serv_sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_childproc(int sig) {
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG); // 임의의 자식 프로세스가 종료되면 pid를 반환하고, 아니면 -1 반환 (블록킹X)
	printf("remove proc id: %d \n", pid);
}

void child_proc_file(int fds) {
	FILE* fp = fopen("echomsg.txt", "wt");
	char msgbuf[BUF_SIZE];
	int i, len;

	for (i = 0; i < 10; i++) { // 클라이언트에서 계속 서비스를 제공하므로, 파일에 어느정도 데이터가 채워지면 파일을 닫도록 하기 위해 반복문으로 구성함
		len = read(fds, msgbuf, BUF_SIZE);
		fwrite((void*)msgbuf, 1, len, fp);
	}
	
	fclose(fp);
}

void child_proc_sock(int sock, int fds, char* buf) { // fds1
	int str_len;
	while ((str_len = read(sock, buf, BUF_SIZE)) != 0) {
		write(sock, buf, str_len);
		write(fds, buf, str_len);
	}
	close(sock);
	puts("client disconnected...");
}
