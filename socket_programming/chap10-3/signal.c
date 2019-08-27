#include <stdio.h>
#include <unistd.h>
#include <signal.h>

// 반드시 반환형이 void이고 매개변수가 int형이어야 한다!
void timeout(int sig) {
	if (sig == SIGALRM) // alarm 함수 호출을 통해 등록된 시간이 되면
		puts("Time out!");
	alarm(2); // SIGALRM 시그널이 2초 단위로 발생됨!
}

void keycontrol(int sig) {
	if (sig == SIGINT) // Ctrl + C가 입력되면
		puts("Ctrl + C pressed!");
}

int main(int argc, char* argv[]) {
	int i;
	// void (*signal(int signo, void (*func)(int)))(int);
	// signal 함수는 매개변수가 signo와 func(함수 포인터) -> timeout 함수 이름과 keycontrol 함수이 올 수 있음
	//               반환형은 (매개변수가 int이고 반환형이 void인 함수 포인터)
	printf("timeout function pointer: %p \n", timeout);
//	signal(SIGALRM, timeout);
	printf("signal return pointer: %p \n", signal(SIGALRM, timeout)); // 이전에 지정한 것이 없기 때문에 nil이 나옴
//	signal(SIGINT, keycontrol);
	printf("keycontrol function pointer: %p \n", keycontrol);
	printf("signal return pointer: %p \n", signal(SIGINT, keycontrol));
	alarm(2); // 여기서 반환은 0 또는 SIGALRM 시그널이 발생하기까지 남아있는 시간을 초 단위로 반환함

	for (i = 0; i < 3; ++i) {
		puts("wait...");
		sleep(100); // 함수 호출은 OS가 하지만, 프로세스가 잠들어 있는 상태에서 함수가 호출될 수는 없다!
			    // 총 300초가 걸릴 것 같지만, 시그널이 발생하면 시그널에 해당하는 시그널 핸들러 호출을 위해
			    // sleep 함수의 호출로 블로킹 상태에 있던 프로세스가 깨어나게 됨!
			    // 그리고 한 번 깨어나면 다시 잠들지 않음 -> 따라서 300초보다 훨씬 적게 걸림!
	}

	return 0;
}
