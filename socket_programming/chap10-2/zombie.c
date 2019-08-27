#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	pid_t pid = fork(); // 자식 프로세스가 생겨나는 시점!
	
	if (pid == 0) // child process
		puts("Hi, I am a child process!");
	else { // parent process
		printf("Child Process Id: %d \n", pid);
		sleep(30); // sleep 30 sec -> 부모 프로세스가 종료되버리면 자식 프로세스도 종료되기 때문!
	}

	if (pid == 0) // child process
		puts("End child process");
	else // parent process
		puts("End parent process");

	return 0; // 자식 프로세스는 여기서 종료된다! -> 그런데 부모 프로세스에서 이것을 받는 코드가 없다!
		  // 따라서 자식 프로세스는 OS에 의해  좀비 프로세스가 된다!
}
