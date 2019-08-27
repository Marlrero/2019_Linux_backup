#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);
void read_childproc(int sig); // signal handler (반환 void, 매개 int)

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock; // socket file descriptor
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	
	int str_len, state;
	char buf[BUF_SIZE];

	pid_t pid;
	struct sigaction act;
	
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	// register action
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	// create socket
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // tcp
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// binding, create listening socket
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error!");
	if (listen(serv_sock, 5) == -1) // 연결 요청 대기 큐 5
		error_handling("listen() error!");
	
	while (1) {
		adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz); // data socket
		if (clnt_sock == -1) // aceept error
			continue;
		else
			puts("new client connected...");

		pid = fork();
		if (pid == -1) { // fock error
			close(clnt_sock);
			continue;
		}

		if (pid == 0) { // child process
			close(serv_sock); // 자식 프로세스는 listening socket이 필요 없음!
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len); // echo

			close(clnt_sock);
			puts("client disconnected...");
			return 0; // child process return 0
		} else // parent process
			close(clnt_sock); // 부모 프로세스는 data socket이 필요 없음!
	}

	close(serv_sock);
	return 0;
}

void read_childproc(int sig) {
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG); // 자식 프로세스가 종료될 때 자식 프로세스의 pid값을 가져옴
	printf("removed proc id: %d \n", pid);
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
