#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	struct sockaddr_in addr1, addr2;
	char* str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr = htonl(0x1020304); // 1.2.3.4
	addr2.sin_addr.s_addr = htonl(0x1010101); // 1.1.1.1
	
	// char* inet_ntoa(struct in_addr adr);
	// 문자열의 주소 값이 반환됨에 주의! -> 문자열 정보를 가급적 메모리 공간에 복사해 두는 것을 권장
	str_ptr = inet_ntoa(addr1.sin_addr);
	strcpy(str_arr, str_ptr);
	printf("Dotted-Decimal notation1: %s \n", str_ptr); // 1.2.3.4가 출력됨
	
	inet_ntoa(addr2.sin_addr);
	printf("Dotted-Decimal notation2: %s \n", str_ptr); // 1.1.1.1이 출력됨
	printf("Dotted-Decimal notation3: %s \n", str_arr); // strcpy한 1.2.3.4가 출력됨
	
	return 0;
}
