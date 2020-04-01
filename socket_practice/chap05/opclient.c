#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPERAND_SIZE sizeof(int)
#define RESULT_SIZE sizeof(int)

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_addr;
	int i, opercnt, result;	
	char message[BUF_SIZE];

	if (argc != 3) {
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error!");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error!");
	else
		puts("connected.....");

	fputs("Operand count: ", stdout);
	scanf("%d", &opercnt);
	message[0] = (char)opercnt; // 1 byte

	for (i = 0; i < opercnt; ++i) {
		printf("Operand %d: ", i + 1);
		scanf("%d", (int*)&message[i * OPERAND_SIZE + 1]);
	}

	fgetc(stdin); // '\n'
	fputs("Operator: ", stdout);
	scanf("%c", &message[opercnt * OPERAND_SIZE + 1]);
	
	write(sock, message, opercnt * OPERAND_SIZE + 2);
	read(sock, &result, RESULT_SIZE); // 사실 4바이트 받을 때까지 while!

	printf("Operation result: %d \n", result);

	close(sock);
	
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
