#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sock; // my socket descriptor
	char msg1[] = "Hi!";
	char msg2[] = "I'm another UDP host!";
	char msg3[] = "Nice to meet you";

	struct sockaddr_in your_adr; // receive host address
	socklen_t your_adr_sz;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		error_handling("socket() error!");
	}

	memset(&your_adr, 0, sizeof(your_adr)); // init sin_zero field
	your_adr.sin_family = AF_INET; // IPv4 address format
	your_adr.sin_addr.s_addr = inet_addr(argv[1]); // convert ipv4 address
	your_adr.sin_port = htons(atoi(argv[2])); // convert port number

	// UDP이므로, sendto가 3번 이뤄졌다면, 받는쪽에서는 recvfrom이 3번 호출되어야 한다!
	// TCP와 다르게, 데이터 경계가 있기 때문이다.
	sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
	sendto(sock, msg2, sizeof(msg2), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));
	sendto(sock, msg3, sizeof(msg3), 0, (struct sockaddr*)&your_adr, sizeof(your_adr));

	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
