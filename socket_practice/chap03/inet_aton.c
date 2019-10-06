#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	char* addr = "127.232.124.79";
	struct sockaddr_in addr_inet;

	if (!inet_aton(addr, &addr_inet.sin_addr))
		printf("Error!\n");
	else
		printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);


	return 0;
}
