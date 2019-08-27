#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// 자식 프로세스를 10개를 차례로 생성하고
// waitpid를 이용하여 차례로 종료하라

#define PROC_NUM 20

int main(int argc, char* argv[]) {
	pid_t pid;
	int i, status;
	
	for (i = 0; i < PROC_NUM; ++i) {
		pid = fork();
		sleep(3);
		if (pid == 0) { // 자식 프로세스 실행 영역
			printf("Child Process %d Execute!\n", i);
			//sleep(3); //3 sec sleep
			return i;
		}
	}

	// 부모 프로세스 실행 영역
	for (i = 0; i < PROC_NUM; ++i) { 
		while (!waitpid(-1, &status, WNOHANG)) {
//			sleep(5);
		}
		sleep(5);	
		if (WIFEXITED(status)) { // 자식 정상 종료 확인
			printf("Child Process %d Kill!\n", WEXITSTATUS(status));
	   	}
	}

	return 0;
}
