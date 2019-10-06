#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_adr;
	char message[BUF_SIZE];

	if (argc != 3) {
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0); // tcp socket
	if (sock == -1)
		error_handling("socket() error!");	

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");
	
	// 전에는 한 번에 읽는 예제였다! -> 불완전한 코드임! -> 충분한 바이트를 받을 때까지 반복해야 한다!
	//                               -> TCP는 경계가 없다고 했다!
	//int str_len = read(sock, message, sizeof(message) - 1);
	
	//if (str_len == -1)
	//	error_handling("read() error!");

	// 이번에는 한 번씩 읽는다! -> 서버는 한 번 보내는데, 나눠서 받는다 -> tcp는 경계가 없다!
	int str_len = 0, read_len, idx = 0;
	while (read_len = read(sock, &message[idx++], 1)) {
		if (read_len == -1)
			error_handling("read() error!");
		str_len += read_len;
	}

	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", str_len);
	
	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
