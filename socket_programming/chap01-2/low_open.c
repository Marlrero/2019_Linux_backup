#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char* message);

int main() {
	int fd; //file descriptor
	char buf[] = "Let's go!\n"; // buffer
	
	fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC);
	// 필요하면 파일을 생성하고 | 쓰기전용으로 파일 오픈 | 기존 데이터를 전부 삭제

	if (fd == -1) {
		error_handling("open() error!");
	}	

	printf("file descriptor: %d \n", fd);
	
	if (write(fd, buf, sizeof(buf)) == -1) {
		error_handling("write() error!");
	}

	close(fd);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
