#include <stdio.h>

int main() {
	int input[8];
	int i, prev, isAscending;
	
	for (i = 0; i < 8; ++i) {
		scanf("%d", &input[i]);
	}
	
	isAscending = (input[0] < input[1]) ? 1 : 0;
	prev = input[1];
	for (i = 2; i < 8; ++i) {
		if (isAscending == 1 && prev < input[i]) {
			isAscending = 1;
		} else if (isAscending == 0 && prev > input[i]) {
			isAscending = 0;
		} else {
			printf("mixed");
			return 0;
		}
		prev = input[i];
	}
	
	if (isAscending) {
		printf("ascending");
	} else {
		printf("descending");
	}

	return 0;
}
