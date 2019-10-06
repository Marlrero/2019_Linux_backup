#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // open()
#include <unistd.h> // close()

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]) {
	int fd; // file descriptor
	char buf[BUF_SIZE];

	fd = open("data.txt", O_RDONLY);
	if (fd == -1)
		error_handling("open() error!");
	printf("file descriptor: %d \n", fd);

	if (read(fd, buf, sizeof(buf)) == -1) // error -1 / success write bytes
		error_handling("read() error!");
	printf("file data: %s", buf);
	
	close(fd);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
