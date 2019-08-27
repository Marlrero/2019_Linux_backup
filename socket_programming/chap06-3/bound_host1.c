#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message);

/*
	이 예제는 서비스를 제공하거나 요청하지 않기 때문에
	여기서는 그냥 my host, your host라고 했음
*/
int main(int argc, char* argv[]) {
	int sock; // host1 socket descriptor
	char message[BUF_SIZE];
	struct sockaddr_in my_adr, your_adr; // my host socket, your host socket
	socklen_t adr_sz; // address size
	int str_len, i;

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) { // create UDP socket
		error_handling("socket() error!");
	}

	memset(&my_adr, 0, sizeof(my_adr)); // init sin_zero field
	my_adr.sin_family = AF_INET; // IPv4 address format
	my_adr.sin_addr.s_addr = htonl(INADDR_ANY); // my ip address
	my_adr.sin_port = htons(atoi(argv[1])); // main argument port number

	if (bind(sock, (struct sockaddr*)&my_adr, sizeof(my_adr)) == -1) {
		error_handling("bind() error!");
	}

	// 주목! UDP는 데이터의 경계가 있다! 따라서, recvfrom을 3번했으면, your_host에서도 3번 sendto해야 한다!
	for (i = 0; i < 3; ++i) {
		sleep(5); // delay 5 sec
		// 5초간 지연시키면, recvfrom이 1번 호출될 때, sendto가 3번 호출되어서 TCP라면 한꺼번에 받을 수 있다.
		// 즉, 이미 보낸 쪽 입장에서는 모두 보낼 수도 있다.
		// 하지만, UDP는 데이터 경계가 있으므로 3번에 recvfrom이 3번 정확히 호출되어야 한다.
		
		adr_sz = sizeof(your_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&your_adr, &adr_sz);
		
		printf("Message %d: %s\n", i + 1, message);
	}
	
	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
