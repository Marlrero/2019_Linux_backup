#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100 // 쓰레드를 100개 생성

void* thread_inc(void* arg); // 이 쓰레드 메인은 1을 증가시킴
void* thread_des(void* arg); // 이 쓰레드 메인은 1을 감소시킴

long long num = 0; // 쓰레드 100개가 동시에 접근하는 데이터 영역

int main(int argc, char* argv[]) {
	pthread_t thread_id[NUM_THREAD]; // 쓰레드 100개가 저장되어 있는 배열
	int i;

	printf("sizeof long long: %d \n", sizeof(long long)); // long long 타입의 크기 확인

	for (i = 0; i < NUM_THREAD; ++i) {
		if (i % 2) // 쓰레드 50개는 1을 증가시킴
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else // 쓰레드 50개는 1을 감소시킴
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);

		//pthread_join(thread_id[i], NULL); // 쓰레드 100개가 끝날 때까지 프로세스 대기
	}

	for (i = 0; i < NUM_THREAD; ++i) {
		pthread_join(thread_id[i], NULL);
	}

	// 우리가 원하는 num의 결과는 0일 것이다! 과연??
	printf("result: %lld \n", num);

	return 0;
}

void* thread_inc(void* arg) {
	int i;

	for (i = 0; i < 50000000; ++i) {
		num += 1;
	}

	return NULL;
}

void* thread_des(void* arg) {
	int i;
	
	for (i = 0; i < 50000000; ++i) {
		num -= 1;
	}

	return NULL;
}
