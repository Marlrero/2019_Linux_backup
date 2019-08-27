#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
	int fds[2]; // file descriptor
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if (pid == 0) { // child
		write(fds[1], str1, sizeof(str1)); // 부모 프로세스에게 보낼 것임 -> 21줄
		sleep(2); // 주목! -> 이게 없다면, 바로 아래에서 fds[1] -> fds[0] 받을 것이다
		read(fds[0], buf, BUF_SIZE); // 부모 프로세스에서 받음 -> 23줄
		printf("Child proc output: %s \n", buf);
	} else { // parent
		read(fds[0], buf, BUF_SIZE); // 자식 프로세스에서 보낸 것을 받음 -> 16줄
		printf("Parent proc output: %s \n", buf);
		write(fds[1], str2, sizeof(str2)); // 자식 프로세스에게 보낼 것임 -> 18줄
		sleep(3); // 부모 프로세스가 먼저 종료되어, 어색한 상황 방지 (이게 없어도 자식 프로세스는 자신의 일을 진행함)
	}

	return 0;
}
