#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void *read(void *arg);
void *accu(void *arg);

static sem_t sem_one;
static sem_t sem_two;

static int num;

int main(int argc, char *argv[]) {
	pthread_t id_t1, id_t2;
	sem_init(&sem_one, 0, 0); // sem_one은 0이므로 통과 불가!
	sem_init(&sem_two, 0, 1); // sem_two는 1을 줄이면서(wait) 통과 가능

	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	sem_destroy(&sem_one);
	sem_destroy(&sem_two);

	return 0;
}

void *read(void *arg) {
	int i;

	for (i = 0; i < 5; i++) {
		fputs("Input num: ", stdout);

		sem_wait(&sem_two); // sem_two가 1이 0이되면서, 임계영역 실행
		scanf("%d", &num);
		sem_post(&sem_one); // sem_one이 0에서 1이되면서, accu 함수에서 실행 가능해짐
	}

	return NULL;
}

void *accu(void *arg) {
	int sum = 0, i;
	for (i = 0; i < 5; i++) {
		sem_wait(&sem_one); // 제일 처음에는 sem_one이 0이므로 실행 불가
				    // read 함수에서 sem_post(&sem_one)을 해줘야 실행 가능
		sum += num;
		sem_post(&sem_two); // sem_two가 0에서 1이되면서, read 함수에서 실행 가능해짐		
	}

	printf("result: %d \n", sum);
	return NULL;
}
