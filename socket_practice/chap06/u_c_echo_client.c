#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_addr, from_addr;
	char message[BUF_SIZE];

	if (argc != 3) {
		printf("Usage: %s <ip> <port> \n", argv[0]);
		exit(1);
	}

	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		error_handling("socket() error!");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

//	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
//		error_handling("connect() error!");
//	}
/*
	read(sock, buffer, BUF_SIZE);
	printf("Message from server: %s \n", buffer);
*/

	connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	int str_len, recv_len, recv_cnt;
	socklen_t addr_size;
	while (1) {
		fputs("Input (q/Q quit) >> ", stdout);
		fgets(message, BUF_SIZE, stdin); // add null

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
			break;
		}

//		write(sock, message, strlen(message));
//		str_len = read(sock, message, BUF_SIZE - 1); // not add null

//		str_len = write(sock, message, strlen(message));
//		recv_len = 0;

/*		while (recv_len < str_len) {
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
			if (recv_cnt == -1)
				error_handling("read() error!");
			recv_len += recv_cnt;		
		}
*/
//		sendto(sock, message, strlen(message), 0,
//			(struct sockaddr*) &serv_addr, sizeof(serv_addr));
//		addr_size = sizeof(from_addr);
//		str_len = recvfrom(sock, message, BUF_SIZE, 0,
//			(struct sockaddr*) &from_addr, &addr_size);

		write(sock, message, strlen(message));
		str_len = read(sock, message, sizeof(message) - 1);

		message[str_len] = 0;
		
		printf("Message from server: %s", message);
	}

	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
