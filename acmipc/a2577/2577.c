#include <stdio.h>

int main() {
	int input;
	int freq[10] = { 0 };
	int i, result = 1;
	
	for (i = 0; i < 3; ++i) {
		scanf("%d", &input);
		result *= input;
	}

	while (result / 10) {
		freq[result % 10]++;	
		result /= 10;
	}
	
	freq[result]++;
	
	for (i = 0; i < 10; ++i) {
		printf("%d\n", freq[i]);
	}

	return 0;
}
