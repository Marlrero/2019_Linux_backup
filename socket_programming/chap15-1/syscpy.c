#include <stdio.h>
#include <fcntl.h>

#define BUF_SIZE 3 // 버퍼의 크기를 최소로 함

int main(int argc, char* argv[]) {
	int fd1, fd2; // file descriptor
	int len;
	char buf[BUF_SIZE];

	fd1 = open("news.txt", O_RDONLY);
	fd2 = open("cpy.txt", O_WRONLY|O_CREAT|O_TRUNC);

	while ((len = read(fd1, buf, sizeof(buf))) > 0) {
		write(fd2, buf, len);
	}

	close(fd1);
	close(fd2);

	return 0;
}
