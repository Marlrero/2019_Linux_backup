#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
	int sock;
	char message[BUF_SIZE];
	struct sockaddr_in my_addr, your_addr;
	
	return 0;
}
