#include <stdio.h>
#include <limits.h>

int main() {
	int arr[9];
	int i, max_pos = -1, max_val = INT_MIN;

	for (i = 0; i < 9; ++i) {
		scanf("%d", &arr[i]);
	}

	for (i = 0; i < 9; ++i) {
		if (max_val < arr[i]) {
			max_val = arr[i];
			max_pos = i;
		}
	}

	printf("%d\n%d\n", max_val, max_pos + 1); 

	return 0;
}
