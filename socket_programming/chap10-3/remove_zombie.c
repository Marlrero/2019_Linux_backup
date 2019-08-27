#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig) {
	int status;
	pid_t id = waitpid(-1, &status, WNOHANG); // 이 함수(블로킹상태에 있지 않음)를 호출해야 자식 프로세스가 좀비가 X
						  // 0을 반환한다는 의미는 어떠한 자식 프로세스도 없다는 의미

	if (WIFEXITED(status)) { // 자식 프로세스가 정상 종료되면
		printf("Removed proc id: %d \n", id);
		printf("Child send: %d \n", WEXITSTATUS(status)); // 값을 출력
	}
}

int main(int argc, char* argv[]) {
	pid_t pid;
	struct sigaction act;

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGCHLD, &act, 0); // 자식 프로세스가 종료되면 act를 시그널 핸들러로 등록함
	
	pid = fork();
	if (pid == 0) { // 자식 프로세스 실행 영역
		puts("Hi! I'm child process!");
		sleep(10);
		return 12; // 종료되면, act함수가 호출되어 12를 출력할 것이다.
	} else { // 부모 프로세스 실행 영역
		printf("Child proc id: %d \n", pid); // 위의 영역의 자식 프로세스 id가 출력될 것이다.
		pid = fork();
		if (pid == 0) { // 또 다른 자식 프로세스 실행 영역
			puts("Hi! I'm another child process!");
			sleep(10);
			exit(24); // 종료되면, act함수가 호출되어 24를 출력할 것이다.
		} else {
			int i;
			printf("Child proc id: %d \n", pid); // 바로 위의 영역의 자식 프로세스 id가 출력될 것이다.
			for (i = 0; i < 5; ++i) {
				puts("wait...");
				sleep(5); // 실제로 25초가 걸리지 않는다. 그 이유는 signal이 발생하면
					// sleep 함수의 호출로 blocking 상태에 있던 프로세스가 깨어남!
			}
		}
	}
	
	return 0;
}
