#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main() {
	int fd1, fd2, fd3; // file descriptor
	
	fd1 = socket(PF_INET, SOCK_STREAM, 0);
	fd2 = open("test.dat", O_CREAT|O_WRONLY|O_TRUNC); // 필요하면 데이터 생성|쓰기전용|기존 데이터 전부 삭제
	fd3 = socket(PF_INET, SOCK_DGRAM, 0);

	printf("file descriptor 1: %d\n", fd1);
	printf("file descriptor 2: %d\n", fd2);
	printf("file descriptor 3: %d\n", fd3);

	close(fd1);
	close(fd2);
	close(fd3);

	return 0;
}
