#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t pid = fork(); // 자식 프로세스 생성

	if (pid == 0) { // 자식 프로세스
		sleep(15); // 자식 프로세스가 15초 동안 잠
		return 24;
	} else { // 부모 프로세스
		// pid_t pid가 -1이면 wait 함수와 마찬가지로 임의의 자식 프로세스가 종료됨을 의미
		// int options가 WNOHANG이면 종료된 자식 프로세스가 존재하지 않아도 블로킹 상태에 있지 않고 0 반환
		while (!waitpid(-1, &status, WNOHANG)) {
			// waitpid 함수가 0을 반환하면 아래 문장이 실행됨 -> blocking 되지 않는다는 의미!
			sleep(3); // 부모 프로세스는 3초간 잠
			puts("parent: sleep 3 sec...");
		}

		if (WIFEXITED(status)) // 자식 프로세스 정상 종료 확인
			printf("Child send %d \n", WEXITSTATUS(status)); // 자식 프로세스의 전달 값 반환
	}

	return 0;
}
