#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main() {
	int fd[3];
	fd[0] = socket(PF_INET, SOCK_STREAM, 0);
	fd[1] = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
	fd[2] = socket(PF_INET, SOCK_DGRAM, 0);
	
	int i;
	for (i = 0; i < 3; ++i) {
		printf("File descriptor %d: %d \n", i + 1, fd[i]);
	}	

	return 0;
}
