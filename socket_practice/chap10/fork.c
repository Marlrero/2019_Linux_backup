#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char* argv[]) {
	pid_t pid;
	int lval = 2;
	gval++, lval += 5;

	pid = fork();
	if (pid == 0) // child
		gval += 2, lval += 2;
	else // parent
		gval -= 2, lval -= 2;

	if (pid == 0)
		printf("Child process: [%d, %d] \n", gval, lval);
	else
		printf("Parent process: [%d, %d] \n", gval, lval);

	return 0;
}
