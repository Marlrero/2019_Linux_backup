#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
	int fds1[2], fds2[2]; // pipe2.c에서 양방향 통신을 이렇게 바꾸면 문제 없음!
	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds1), pipe(fds2);
	pid = fork();
	if (pid == 0) { // child
		write(fds1[1], str1, sizeof(str1)); // 부모 프로세스의  fds1[0]에게 보냄
		read(fds2[0], buf, BUF_SIZE); // 부모 프로세스에서 fds2[1]에서 읽어들임
		printf("Child proc output: %s \n", buf);
	} else { // parent
		read(fds1[0], buf, BUF_SIZE); // 자식 프로세스에 fds1[1]에서 읽어들임
		printf("Parent proc output: %s \n", buf);
		write(fds2[1], str2, sizeof(str2)); // 자식 프로세스의 fds2[0]에게 보냄
		sleep(3); // 부모 프로세스가 먼저 종료되는 어색한 상황 방지
	}

	return 0;
}
