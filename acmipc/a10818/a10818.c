#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
	int size, i, max = INT_MIN, min = INT_MAX;
	int* arr;	

	scanf("%d", &size);

	arr = (int*) malloc(sizeof(int) * size);

	for (i = 0; i < size; ++i) {
		scanf("%d", &arr[i]);
	}

	for (i = 0; i < size; ++i) {
		if (max < arr[i]) {
			max = arr[i];
		}
		if (min > arr[i]) {
			min = arr[i];
		}
	}

	printf("%d %d", min, max);

	free(arr);
	arr = NULL;

	return 0;
}
