#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char* message);

#define BUF_SIZE 1024

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_addr; //서버 주소
//	char message[30]; //메시지
//	int str_len; //메시지 크기

	if (argc != 3) { //메인함수 인자가 3이 아니면
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}	

	// 1. int socket(int domain, int type, int protocol);
	sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓 생성
	if (sock == -1) { // 소켓 생성 시 오류 발생
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); // 서버 주소에 4 bytes(32 bits -> IPv4) 할당!
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	// 2. int connect(int sockfd, struct sockaddr* serv_addr, socklen_t addrlen);
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect() error!");
	} // socket 생성 후 bind, listen 함수 호출이 이어지면 서버 소켓이 되고
	  // socket 생성 후 connect 함수 호출이 이어지면 클라이언트 소켓이 됨
	
//	str_len = read(sock, message, sizeof(message) - 1); // 서버로부터 보낸 메시지를 읽는다.
//	if (str_len == -1) { // read 함수 오류
//		error_handling("read() error!");
//	}

//	printf("Message from server: %s \n", message);
	int i, recv_len, recv_cnt, str_len;
	char input[BUF_SIZE - sizeof(int)];
	char message[BUF_SIZE];
	
	for (i = 0; i < 3; ++i) { // 서버와 3번 주고받는다!
		printf("client가 보낼 메시지 입력 >> ");
		scanf("%[^\n]s", input);
		
		//char* message = (char*) malloc(sizeof(char) * strlen(input) + sizeof(int));
		
		*message = (char)strlen(input); // 문자열 길이
		strcpy(message + sizeof(int), input); // 문자열 데이터
	
		write(sock, message, BUF_SIZE - 1);
	
		memset(message, 0, BUF_SIZE); // 재사용하기 위해 초기화
		recv_len = recv_cnt = 0;

		// 문자열 길이를 서버로부터 읽어들인다.
		str_len = read(sock, message, sizeof(int));

		// 실제 데이터를 읽는다.
		while (recv_len <= str_len) {
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		printf("server가 보낸 메시지: %s \n", message);

		memset(message, 0, BUF_SIZE); // 재사용하기 위해 초기화
		// 메모리 할당 해제
		//free(message);
		//message = NULL;
		getchar(); // 개행 문자 제거
	}

	close(sock); // 소켓 닫기
        
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr); //표준 오류 출력 스트림으로 메시지를 출력
	fputc('\n', stderr);
	exit(1);
}
