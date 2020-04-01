#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");

	alarm(2);
}

void keycontrol(int sig) {
	if (sig == SIGINT)
		puts("CTRL+C pressed");
}

int main(int argc, char* argv[]) {
	int i;
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol); // 시그널 등록
	alarm(2); // 2초마다 SIGALRM 발생 등록

	for (i = 0; i < 3; i++) {
		puts("wait...");
		sleep(100); // SIGALRM이 오면 깨어남!
	}

	return 0;
}
