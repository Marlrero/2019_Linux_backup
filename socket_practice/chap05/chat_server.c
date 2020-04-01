#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int listen_sock, data_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	char message[] = "Hello World!";

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
/*
	if ((data_sock = accept(listen_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size)) == -1) {
		error_handling("accept() error!");
	}

	write(data_sock, message, sizeof(message));
	
	close(data_sock);
*/
/*
	int i, str_len;
	for (i = 0; i < 5; ++i) {
		if ((data_sock = accept(listen_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size)) == -1) {
			error_handling("accept() error!");
		} else {
			printf("Connected client: %d \n", i + 1);
		}

		while ((str_len = read(data_sock, message, BUF_SIZE)) != 0) {
			write(data_sock, message, str_len); // echo
		}
		
		close(data_sock);
	}
*/

	if ((data_sock = accept(listen_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size)) == -1) {
		error_handling("accept() error!");
	} else {
		printf("Connected! .... \n");
	}

	int str_len, recv_cnt, recv_len;
	while (1) {
		recv_cnt = recv_len = 0;
		// 몇 바이트를 받은 것인지 모름
		// read할 때 1바이트로 클라이언트가 보낼 메시지 바이트를 읽어들이고, 그리고 읽어들인다!
		str_len = read(data_sock, message, BUF_SIZE - 1);
		message[str_len] = 0;

		printf("Message from client: %s", message);

		fputs("Input (q/Q quit) >> ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
			break;
		}

		write(data_sock, message, strlen(message));
	}
	close(data_sock);
	close(listen_sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
