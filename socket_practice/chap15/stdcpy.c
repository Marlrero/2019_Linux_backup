#include <stdio.h>
//#include <unistd.h>
//#include <fcntl.h>

#define BUF_SIZE 3

int main(int argc, char* argv[]) {
//	int fd1, fd2;
	FILE* fp1, *fp2;
//	int len;
	char buf[BUF_SIZE];

//	fd1 = open("news.txt", O_RDONLY);
//	fd2 = open("cpy.txt", O_WRONLY|O_CREAT|O_TRUNC);

	fp1 = fopen("news.txt", "r");
	fp2 = fopen("cpy.txt", "w");

//	while ((len = read(fd1, buf, sizeof(buf))) > 0)
//		write(fd2, buf, len);

	while (fgets(buf, BUF_SIZE, fp1) != NULL)
		fputs(buf, fp2);

//	close(fd1);
//	close(fd2);

	fclose(fp1);
	fclose(fp2);

	return 0;
}
