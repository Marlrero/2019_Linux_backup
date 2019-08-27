#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sock; //client socket
	struct sockaddr_in serv_addr; //server address
	char message[30];
	int str_len = 0;
	int idx = 0, read_len = 0;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_STREAM, 0); // create socket
	if (sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); // init serv_addr
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	// connect server
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect() error");
	}
	
	// 주목! 이전 예제와 달리, 하나씩 read하고 있음
	while (read_len = read(sock, &message[idx++], 1)) {
		if (read_len == -1) {
			error_handling("read() error");
		}
		
		str_len += read_len;
	}
	
	// TCP는 전송되는 데이터 경계(boundary)가 존재하지 않음!!
	printf("Message from server: %s \n", message);
	printf("Function read call count: %d \n", str_len); // read 함수 호출 횟수
	close(sock);
	
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
