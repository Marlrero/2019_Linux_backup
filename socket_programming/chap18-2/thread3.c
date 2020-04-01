#include <stdio.h>
#include <pthread.h>

void* thread_summation(void* arg);
int sum = 0;

int main(int argc, char* argv[]) {
	pthread_t id_t1, id_t2;
	int range1[] = { 1, 5 };
	int range2[] = { 6, 10 };

	pthread_create(&id_t1, NULL, thread_summation, (void*)range1); // 1 ~ 5까지 더하는 쓰레드
	pthread_create(&id_t2, NULL, thread_summation, (void*)range2); // 6 ~ 10까지 더하는 쓰레드

	pthread_join(id_t1, NULL); // id_t1 쓰레드가 종료될 때까지 프로세스를 대기 상태에 둠
	pthread_join(id_t2, NULL); // id_t2 쓰레드가 종료될 때까지 프로세스를 대기 상태에 둠
	
	// 1 ~ 10까지 더하는 프로세스 -> 멀티 쓰레드로!
	printf("result: %d \n", sum);

	return 0;
}

void* thread_summation(void* arg) {
	int start = ((int*)arg)[0];
	int end = ((int*)arg)[1];

	while (start <= end) {
		sum += start;
		start++;
	}

	return NULL;
}
