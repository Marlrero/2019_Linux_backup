#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 1024

void error_handling(char* message);

void read_childproc(int sig) {
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		printf("Removed proc id: %d \n", pid);
		printf("Child send: %d \n", WEXITSTATUS(status));
	}
}

int main(int argc, char* argv[]) {
	int listen_sock, data_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	//char message[] = "Hello World!";
	char message[BUF_SIZE];

	struct sigaction act;
	int state;
	pid_t pid;

	int fds[2]; // pipe
	pipe(fds);

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	if (argc != 2) {
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // own ip
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(listen_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error!");
	}

	if (listen(listen_sock, 5) == -1) {
		error_handling("listen() error!");
	}

	pid = fork();
	if (pid == 0) {
		FILE* fp = fopen("echomsg.txt", "wb");
		char msgbuf[BUF_SIZE];
		int i, len;

		for (i = 0; i < 5; i++) {
			len = read(fds[0], msgbuf, BUF_SIZE); // 다른 자식 프로세스에게 읽은 문자열을 받음
			fwrite((void*)msgbuf, 1, len, fp);
		}

		fclose(fp);
		return 0;
	}


	int i, str_len;
	while (1) {
		clnt_addr_size = sizeof(clnt_addr);
		if ((data_sock = accept(listen_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size)) == -1) {
			continue;
		} else {
			printf("Connected client.... \n");
		}

		pid = fork(); // 자식 프로세스 생성
		if (pid == -1) {
			close(data_sock);
			continue;
		}

		if (pid == 0) { // 자식 프로세스 실행 영역
			close(listen_sock); // 자식은 리스닝 소켓 필요 없다!
			
			while ((str_len = read(data_sock, message, BUF_SIZE)) != 0) {
				write(data_sock, message, str_len); // echo
				write(fds[1], message, str_len); // 다른 자식 프로세스에게 읽은 문자열을 보냄!
			}

			close(data_sock); // 이젠 데이터 소켓 필요 없다!
			return 0;	
		} else { // 부모 프로세스 실행 영역
			close(data_sock); // 부모는 데이터 소켓 필요 없다!
			continue; // 부모는 listen 상태가 되어야 함
		}
	} // 무한 루프 코드임!

	close(listen_sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
