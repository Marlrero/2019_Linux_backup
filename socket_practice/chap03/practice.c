#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
	//1.  serv_adr.sin_addr.s_addr = inet_addr("100.100.100.0");
	// -> inet_aton() 함수를 이용해서 고쳐보시오.
	
	//int serv_sock;
	struct sockaddr_in serv_adr;
	
	inet_aton("100.100.100.0", &serv_adr.sin_addr);	

	//2. int r = inet_aton(argv[1], &serv_adr.sin_addr);
	// -> inet_addr() 함수로 고쳐 써 보시오.

	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

	return 0;	
}
