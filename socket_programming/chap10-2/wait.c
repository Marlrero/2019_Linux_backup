#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t pid = fork();

	if (pid == 0) // child process
		return 3; // 자식 프로세스는 부모 프로세스에게 3을 넘겨준다! (부모 프로세스가 이를 받아야 하는 코드가 있어야 함)
	else { // parent process
		printf("Child PID: %d \n", pid);
		pid = fork(); // child process가 또 생성됨!
		
		if (pid == 0)
			exit(7); // return이나 exit의 인자나 마찬가지!
		else {
			printf("Child PID: %d \n", pid);
			wait(&status); // 여기서 받는다!
			if (WIFEXITED(status)) // 자식 프로세스가 정상 종료 했나?
				printf("Child send one: %d \n", WEXITSTATUS(status));

			wait(&status); // 여기서 받는다!
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));

			sleep(30); // 부모 프로세스가 종료되면 안되기 때문! (다같이 종료됨)
		}
	}
	
	return 0;
}
