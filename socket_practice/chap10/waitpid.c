#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t pid = fork();

	if (pid == 0) { // child process
		sleep(15);
		return 24;
	} else { // parent process
		while (!waitpid(-1, &status, WNOHANG)) { // -1: 임의의 자식
							 // WNOHANG: with no hang 기다림X
			sleep(3);
			puts("sleep 3sec");
		}

		if (WIFEXITED(status)) // wait if exited
			printf("child send %d \n", WEXITSTATUS(status)); // wait exit status
	}

	return 0;
}
