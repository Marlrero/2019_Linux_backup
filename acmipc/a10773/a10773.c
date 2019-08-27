#include <stdio.h>
#include <stdlib.h>

void push(int* arr, int size, int* pos, int value);
void pop(int* arr, int size, int* pos);
int empty(int* arr, int* pos);
int sum(int* arr, int pos);

int main() {
	int i, value, size, pos = -1;
	int* arr;

	scanf("%d", &size);
	arr = (int*) malloc(sizeof(int) * size);

	for (i = 0; i < size; ++i) {
		scanf("%d", &value);
		if (value == 0) {
			pop(arr, size, &pos);
		} else {
			push(arr, size, &pos, value);
		}
	} 

	printf("%d", sum(arr, pos));

	free(arr);
	arr = NULL;

	return 0;
}

void push(int* arr, int size, int* pos, int value) {
	if (size == *pos + 1) {
		return;
	}
	arr[++(*pos)] = value;
}

void pop(int* arr, int size, int* pos) {
	if (empty(arr, pos)) {
		return;
	}
	(*pos)--;
}

int empty(int* arr, int* pos) {
	return (*pos == -1) ? 1 : 0;
}

int sum(int* arr, int pos) {
	int i, sum = 0;
	for (i = 0; i <= pos; ++i) {
		sum += arr[i];
	}

	return sum;
}
