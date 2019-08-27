#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <sys/socket.h>
#include <netdb.h> // hostent 자료형과 gethostbyname()

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int i;
	struct hostent* host; // 주목!
	
	if (argc != 2) {
		printf("Usage: %s <HostName>\n", argv[0]);
		exit(1);
	}

	host = gethostbyname(argv[1]); // host name -> hostent 구조체
	// default dns에게 먼저 물어보고, 이후 재귀적 질의로 찾아냄!

	if (!host) { // host == NULL이면
		error_handling("gethost... error!"); // host를 찾지 못함!
	}

	printf("Canonical Name: %s\n", host->h_name); // *(host).h_name하고 같은 뜻! (C문법)
	
	for (i = 0; host->h_aliases[i] != NULL; ++i) { // "!= NULL" 부분을 없애도 된다!
		printf("Aliases Name %d: %s \n", (i + 1), host->h_aliases[i]); 
	}

	printf("Address Type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
	printf("Address Length: %d \n", host->h_length);

	for (i = 0; host->h_addr_list[i] != NULL; ++i) {
		printf("IP Address %d: %s \n", (i + 1),
			inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
