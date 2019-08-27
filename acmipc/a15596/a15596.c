#include <stdio.h>

long long sum(int* a, int n);

int main() {
	return 0;
}

long long sum(int* a, int n) {
	long long i, result = 0;
	for (i = 0; i < n; ++i) {
		result += a[i];
	}
}
