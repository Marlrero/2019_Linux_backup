#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int tcp_sock, udp_sock; // socket(file) descriptor
	int sock_type; // socket type buffer
	socklen_t optlen; // socket type buffer size
	int state; // getsockopt function return

	optlen = sizeof(sock_type); // 4 bytes

	tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
	udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

	if (tcp_sock == -1 || udp_sock == -1) {
		error_handling("socket() error!");
	}

	printf("SOCK_STREAM: %d \n", SOCK_STREAM);
	printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);

	// int getsockopt(int sock, int level, int optname, void* optval, socklen_t* optlen);
	state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state) {
		error_handling("getsockopt() error!");
	}
	printf("Socket type one: %d \n", sock_type);

	state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if (state) {
		error_handling("getsockopt() error!");
	}
	printf("Socket type one: %d \n", sock_type);

	return 0;	
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);	
	exit(1);
}
