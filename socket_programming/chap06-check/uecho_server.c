#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]) {
	int serv_sock; // server socket descriptor
	char message[BUF_SIZE];
	int str_len;

	socklen_t clnt_adr_sz; // client address size
	struct sockaddr_in serv_adr, clnt_adr; // server address, client address	

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0); // create UDP socket
	if (serv_sock == -1) {
		error_handling("socket() error!");
	}

	memset(&serv_adr, 0, sizeof(serv_adr)); // init sin_zero field
	serv_adr.sin_family = AF_INET; // IPv4 address format
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // server address <- host to network long (current)
	serv_adr.sin_port = htons(atoi(argv[1])); // server port <- host to network (main argument)
	
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
		error_handling("bind() error!");
	}

	clnt_adr_sz = sizeof(clnt_adr);
	str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	message[str_len] = 0;
	printf("Message from client: %s", message);

	fputs("Input Message: ", stdout);
	fgets(message, BUF_SIZE, stdin);
	sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);

/*
		fputs("Input Message: ", stdout);
		fgets(message, BUF_SIZE, stdin);
		sendto(serv_sock, message, strlen(message), 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);		

		str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		message[str_len] = 0; // null character
		printf("Client Message: %s", message);
*/
	close(serv_sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
