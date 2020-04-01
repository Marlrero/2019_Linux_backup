#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t pid = fork();

	if (pid == 0) // child process 1
		return 3;
	else { // parent process
		printf("Child pid: %d \n", pid);
		pid = fork();
		if (pid == 0) // child process 2
			exit(7);
		else { // parent process
			printf("Child pid: %d \n", pid);
			wait(&status); // 자식 프로세스 리턴 대기
			if (WIFEXITED(status))
				printf("Child send 1: %d \n", WEXITSTATUS(status));

			wait(&status); // 자식 프로세스 리턴 대기
			if (WIFEXITED(status))
				printf("Child send 2: %d \n", WEXITSTATUS(status));

			sleep(30); // sleep 30 sec
		}
	}

	return 0;
}
