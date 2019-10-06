#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char* argv[]) {
	struct iovec vec[2];
	char buf1[] = "ABCDEFG";
	char buf2[] = "1234567";
	int str_len;

	vec[0].iov_base = buf1;
	vec[0].iov_len = 3;
	vec[1].iov_base = buf2;
	vec[1].iov_len = 4;

	str_len = writev(1, vec, 2);
	// int filedes, const struct iovec* iov, int iovcnt
	// 소켓/파일 디스크립터, 데이터 위치 크기 정보, 두번째 인자 길이
	// 1을 전달했으므로, stdout

	puts("");
	printf("Write bytes: %d \n", str_len);
	return 0;
}
