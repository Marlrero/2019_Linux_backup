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
	struct sockaddr_in serv_addr;
	char buffer[BUF_SIZE];

	if (argc != 3) {
		printf("Usage: %s <ip> <port> \n", argv[0]);
		exit(1);
	}

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect() error!");
	}

	read(sock, buffer, BUF_SIZE);
	printf("Message from server: %s \n", buffer);

	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
