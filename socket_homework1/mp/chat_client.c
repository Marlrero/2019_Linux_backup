#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);
void read_routine(int sock, char* buf);
void write_routine(int sock, char* buf);

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_adr;
	char message[BUF_SIZE];
	pid_t pid;

	if (argc != 3) {
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}	

	sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	pid = fork();
	if (pid == 0) // 자식 프로세스 실행 영역
		write_routine(sock, message);
	else // 부모 프로세스 실행 영역
		read_routine(sock, message);

	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_routine(int sock, char* buf) {
	int msg_len, recv_len, recv_cnt;
	
	while (1) {
		read(sock, &msg_len, 1);

		recv_len = 0;	
		while (recv_len < msg_len) {
			recv_cnt = read(sock, buf, BUF_SIZE);
			if (recv_cnt == 0)
				return;

			recv_len += recv_cnt;
		}

		buf[recv_len] = 0; // null character
		printf("<< %s", buf);
	}
}

void write_routine(int sock, char* buf) {
	while (1) {
		printf(">> ");
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
			shutdown(sock, SHUT_WR); // fork로 인한 파일 디스크립터 복사로 인한 close 함수 호출로 인해 EOF 전달을 기대할 수 없음!
			return;
		}

		char message[BUF_SIZE];
		message[0] = (char)strlen(buf);
		strcpy(&message[1], buf);
		write(sock, message, strlen(message));
	}
}
