#include <stdio.h>

long long fact(long long n) {
	if (n == 0 || n == 1)
		return 1;
	else
		return n * fact( n - 1 );
} 

long double euler(long long n) {
	int loop;
	long double result = 0.0;

	for (loop = 0; loop <= n; loop++) {
		result += 1.0 / (long double)fact(loop);		
	}
	
	return result;
}

int main() {
	long long n;
	printf("입력 >> ");
	scanf("%lld", &n);

	printf("출력 >> %Lf \n", euler(n));	

	return 0;
}
