#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
	int fds[2]; // filedes -> file descriptor
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	// int pipe(int filedes[2]) -> filedes[0]: out (파이프 출구, 데이터 수신)
	//                          -> filedes[1]: in (파이프 입구, 데이터 송신)
	pipe(fds);
	pid = fork();
	if (pid == 0) // child process
		write(fds[1], str, sizeof(str)); // 부모 프로세스의 fds[0]로 보냄
	else {
		read(fds[0], buf, BUF_SIZE); // 자식 프로세스의 fds[1]으로부터 받음
		puts(buf);
	}
	
	return 0;
}
