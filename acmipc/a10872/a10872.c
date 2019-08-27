#include <stdio.h>

//int factorial(int n);

int main() {
	int value, i, result = 1;
	scanf("%d", &value);

	for (i = 1; i <= value; ++i) {
		result *= i;
	}
	printf("%d", result);

	return 0;
}
/*
int factorial(int n) {
	if (n == 1) {
		return 1;
	}
	return n * factorial(n -1);
}
*/
