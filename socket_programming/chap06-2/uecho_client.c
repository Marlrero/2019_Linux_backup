#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sock; // client socket descriptor
	char message[BUF_SIZE];
	int str_len;
	
	socklen_t adr_sz; // server address size
	struct sockaddr_in serv_adr, from_adr;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0); // create UDP socket
	if (sock == -1) {
		error_handling("socket() error!");
	}

	memset(&serv_adr, 0, sizeof(serv_adr)); // init sin_zero field
	serv_adr.sin_family = AF_INET; // IPv4 address format
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]); // convert ip address
	serv_adr.sin_port = htons(atoi(argv[2])); // convert port number

	while (1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		// send to server

		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
		message[str_len] = 0; // null character
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
