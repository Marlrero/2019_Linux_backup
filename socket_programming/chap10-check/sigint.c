#include <stdio.h>
#include <stdlib.h> // sleep()
#include <unistd.h>
#include <signal.h>

void eof(int sig) {
	char ch;
	if (sig == SIGINT) {
		printf("정말로 종료하시겠습니까? (y/n)");
		scanf("\n%c", &ch);

		if (ch == 'y' || ch == 'Y') {
			exit(1);
		} //else {
		//	sleep(1); // 1s wait -> 다시 잠들기
		//}
	}
}

int main(int argc, char* argv[]) {
	struct sigaction act;
	
	act.sa_handler = eof;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0); // Ctrl + Z에 대한 시그널 핸들러 등록!

	while (1) {
		sleep(1);
		printf("...\n");
//		sleep(1);
//		sigaction(SIGINT, &act, 0); // Ctrl + Z에 대한 시그널 핸들러 등록!
	}

	return 0;
}
