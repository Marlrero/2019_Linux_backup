#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char* argv[]) {
	pid_t pid;
	int lval = 20;
	
	gval++, lval += 5; // gval = 11, lval = 25

	pid = fork(); // 부모 프로세스에서 fork 함수의 반환 값은 자식 프로세스 id가 되고,
		      // 자식 프로세스에서 fork 함수의 반환 값은 0이 된다!
	if (pid == 0) // 자식 프로세스에서 실행됨
		gval += 2, lval += 2;  // 자식 프로세스 결과: gval = 13, lval = 27
	else // 부모 프로세스에서 실행됨
		gval -= 2, lval -= 2;  // 부모 프로세스 결과: gval = 9, lval = 23

	if (pid == 0)
		printf("Child Proc: [%d, %d] \n", gval, lval);
	else
		printf("Parent Proc: [%d, %d] \n", gval, lval);

	return 0;
}
