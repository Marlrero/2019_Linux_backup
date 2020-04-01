#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPERAND_SIZE sizeof(int)
#define RESULT_SIZE sizeof(int)

void error_handling(char* message);
int calculate(int opercnt, int* operand, char operator);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	char message[BUF_SIZE];
	socklen_t clnt_addr_size;
	int opercnt, i, result;
	int recv_cnt, recv_len;

	if (argc != 2) {
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling("socket() error!");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error!");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	clnt_addr_size = sizeof(clnt_addr);
	
	for (i = 0; i < 5; ++i) {
		opercnt = 0;
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1)
			error_handling("accept() error!");

		read(clnt_sock, &opercnt, 1); // 1 byte read
		
		recv_len = 0;
		while ((opercnt * OPERAND_SIZE + 1) > recv_len) {
			recv_cnt = read(clnt_sock, &message[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}

		result = calculate(opercnt, (int*)message, message[recv_len - 1]);
		write(clnt_sock, (char*)&result, RESULT_SIZE);

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

int calculate(int opercnt, int* operand, char operator) {
	int result = operand[0], i;
	
	switch(operator) {
		case '+':
			for (i = 1; i < opercnt; ++i)
				result += operand[i];
			break;
		case '-':
			for (i = 1; i < opercnt; ++i)
				result -= operand[i];
			break;
		case '*':
			for (i = 1; i < opercnt; ++i)
				result *= operand[i];
			break;
	}

	return result;
}
