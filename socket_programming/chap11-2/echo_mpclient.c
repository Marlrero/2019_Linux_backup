#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);
void read_routine(int sock, char* buf);
void write_routine(int sock, char* buf);

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_adr;
	char buf[BUF_SIZE];

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

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	pid = fork();
	if (pid == 0) // child process
		write_routine(sock, buf);
	else // parent process
		read_routine(sock, buf);

	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

// 아래 두 함수에서 sock 매개변수는 소켓의 파일 디스크립터가 복사되는 것이며,
// 버퍼는 자식과 부모 각각 1개씩 존재하게 된다!
void read_routine(int sock, char* buf) {
	while (1) {
		int str_len = read(sock, buf, BUF_SIZE);
		if (str_len == 0)
			return;

		buf[str_len] = 0; // null character
		printf("Message from server: %s", buf);
	}
}

void write_routine(int sock, char* buf) {
	while (1) {
		fgets(buf, BUF_SIZE, stdin);
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
			shutdown(sock, SHUT_WR); // 출력 스트림만 종료한다 (half-close)
			/*
				왜 그럴까?
				서버로 EOF 전달을 위해서 shutdown 함수를 호출하는 것이다.
				물론 다음 문장에 return에서 EOF의 전달을 기대할 수 있으나,
				fork로 인해 파일 디스크립터가 복사된 상황이므로, 한번의 close 함수호출로
				EOF 전달을 기대할 수 없기 때문이다.
				따라서, 반드시 shutdown 함수 호출을 통해 EOF 전달을 별도 명시해야 한다.
			 */
			return;
		}
		write(sock, buf, strlen(buf));
	}
}
