#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;

	FD_ZERO(&reads);
	FD_SET(0, &reads); // stdin = 0
	
	/*
	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;
	*/
	// 이 위치에서 timeout 설정을 하면 안됨!
	// select 함수 호출 이후에 구초체 timeval의 멤버들이 타임아웃이 발생하기까지 남았던 시간으로 바뀌기 때문
	// select 함수 호출 전에 timveval를 매번 초기화해줘야 함

	while (1) {
		temps = reads; // select 함수 호출이 끝나면 변화가 생긴 파일 디스크립터 위치를 제외한
				// 나머지 비트들은 0으로 초기화됨! -> 원본 유지를 위해 복사의 과정을 거쳐야 함
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		//int select(int maxfd, fd_set* readset, fd_set* writeset, fd_set* exceptset, const struct timeval* timeout);
		//          검사대상 디스크립터 수, 수신된 데이터 존재여부, 블로킹 없는 데이터 전송 가능 여부, 예외상황 발생 여부, 블로킹 상태에 빠지지 않게 타임 아웃
		// 오류 발생 시 -1이 반환, 타임 아웃 발생 시 0 반환, 관심 대상 변화가 생기면 0보다 큰 값이 반환(변화가 발생한 디스크립터 수)
		result = select(1, &temps, 0, 0, &timeout);
		if (result == -1) {
			puts("select() error!");
			break;
		} else if(result == 0) {
			puts("timeout!");
		} else {
			if (FD_ISSET(0, &temps)) { // temp에서 변화가 보인 파일 디스크립터가 0이면
				str_len = read(0, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console: %s", buf);
			}
		}
	}
	
	return 0;
}
