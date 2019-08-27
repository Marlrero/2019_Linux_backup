#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// 사실 signal 함수는 유닉스 계열 운영체제 별로 동작의 차이가 있어
// sigaction 함수를 실제로 많이 사용한다!

void timeout(int sig) {
	if (sig == SIGALRM)
		puts("Time out!");
	alarm(2); // 2초가 지나서 SIGALRM 시그널 발생
}

int main(int argc, char* argv[]) {
	int i;
	struct sigaction act;
	
	/*
		struct sigaction {
			void (*sa_handler)(int);
			sigset_t sa_mask; -> sigemptyset(sigset_t* sa_mask);를 이용해 0으로 초기화함!
			int sa_flags;
		}
	 */
	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask); // &(act.sa_mask) -> .연산자가 먼저 우선순위!
	// C언어 연산자: 괄호, 포인터, 도트 > 단항 > 산술 > 비트 이동 > 관계 > 비트논리 > 논리 > 조건 > 대입, 복합대입 > 콤마
	act.sa_flags = 0;

	// int sigaction(int signo, const struct sigaction* act, struct sigaction* oldact);
	sigaction(SIGALRM, &act, 0);

	alarm(2); // 2초 뒤에 SIGALRM 발생 예약
	
	for (i = 0; i < 3; i++) {
		puts("wait...");
		sleep(100);
	}
	
	return 0;
}
