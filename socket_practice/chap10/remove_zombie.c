#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig) {
	int status;
	pid_t child_id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status)) {
		printf("Removed process id: %d \n", child_id);
		printf("Child send: %d \n", WEXITSTATUS(status));
	}
}

int main(int argc, char* argv[]) {

	struct sigaction act;
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0);


//	signal(SIGCHLD, read_childproc);

	pid_t pid = fork();

	if (pid == 0)
		puts("Hi, I am a child process..");
	else {
		printf("Child process ID: %d \n", pid);
		sleep(30);
	}

	if (pid == 0)
		puts("End child process..");
	else
		puts("End parent process..");

	return 0;
}
