#include <stdio.h>

void fibo_func(int num);

int main() {
	int num;

	printf("출력할 피보나치 수 입력? (0보다 큰 값)");
	do {
		scanf("%d", &num);
	} while (num <= 0);

	fibo_func(num);

	return 0;
}
// 0 1 1 2
void fibo_func(int num) {
	int n1 = 0, n2 = 1, n3, i;

	if (num == 1) {
		printf("%4d, ", n1);
	}
	else {
		printf("%4d, %4d, ", n1, n2);
	}

	for (i = 2; i <= num; i++) {
		n3 = n1 + n2;
		printf("%4d, ", n3);
		n1 = n2; n2 = n3;
		i = (n1 + n2 <= num) ? n3 : n1 + n2;	
	}
	printf("\n");
}
