#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);
void read_routine(int socket, char* message);
void write_routine(int socket, char* message);

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];

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
	
	pid_t pid = fork();
	if (pid == 0)
		write_routine(sock, message);
	else
		read_routine(sock, message);

	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_routine(int socket, char* message) {
	while (1) {
		int str_len = read(sock, message, BUF_SIZE - 1);
		if (str_len == 0)
			return;		
	}
		
	printf("Message from server: %s", message);

}

void write_routine(int socket, char* message) {
	while (1) {
		fputs("Input (q/Q quit) >> ", stdout);
		fgets(message, BUF_SIZE, stdin); // add null

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
			return;
		}

		write(socket, message, strlen(message));
	}
}
