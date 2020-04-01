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

	// 소켓 디스크립터 -> 파일 포인터
	FILE* readfp = fdopen(sock, "r");
	FILE* writefp = fdopen(sock, "w"); // 입출력 분리

	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect() error!");
	}
	
	int str_len, recv_len, recv_cnt;
	while (1) {
		fputs("Input (q/Q quit) >> ", stdout);
		fgets(message, BUF_SIZE, stdin); // add null

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
			break;
		}

/*
		str_len = write(sock, message, strlen(message));
		recv_len = 0;

		while (recv_len < str_len) {
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
			if (recv_cnt == -1)
				error_handling("read() error!");
			recv_len += recv_cnt;		
		}

		message[str_len] = 0;
*/
		fputs(message, writefp);
		fflush(writefp); // 소켓으로 데이터 송신

		fgets(message, BUF_SIZE, readfp); // 소켓으로 데이터 수신
				
		printf("Message from server: %s", message);
	}

	//close(sock);
	fclose(readfp);
	fclose(writefp);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
