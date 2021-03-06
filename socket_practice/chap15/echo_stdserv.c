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
	FILE *readfp, *writefp;

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

	int i, str_len;
	for (i = 0; i < 5; ++i) {
		if ((data_sock = accept(listen_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size)) == -1) {
			error_handling("accept() error!");
		} else {
			printf("Connected client: %d \n", i + 1);
		}

		readfp = fdopen(data_sock, "r");
		writefp = fdopen(data_sock, "w");

/*		while ((str_len = read(data_sock, message, BUF_SIZE)) != 0) {
			write(data_sock, message, str_len); // echo
		}*/

		while (!feof(readfp)) {
			fgets(message, BUF_SIZE, readfp);
			fputs(message, writefp);
			fflush(writefp);
		}
		
//		close(data_sock);

		fclose(readfp);
		fclose(writefp);
	}

	close(listen_sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
