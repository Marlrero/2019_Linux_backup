#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock; // 리스닝 소켓과 실제 데이터를 전송하는 소켓 디스크립터
	char message[BUF_SIZE];
	int str_len, i;

	struct sockaddr_in serv_adr, clnt_adr; // 서버 주소와 클라이언트 주소를 저장하는 구조체
	socklen_t clnt_adr_sz; // 클라이언트 주소 크기

	if (argc != 2) { // main 함수인자가 2개가 아니면
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 리스닝 소켓을 만들기 위해, IPv4 - TCP로 소켓 생성
	if (serv_sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr)); // serv_adr에서 sin_zero 필드를 0으로 초기화하기 위함
	serv_adr.sin_family = AF_INET; // IPv4 주소를 사용
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // host to network long -> INADDR_ANY는 현재 이 호스트의 주소를 서버 주소로 할당
	serv_adr.sin_port = htons(atoi(argv[1])); // main 함수의 인자 포트를 atoi를 이용해 int로 바꾸고 이를 다시 host to network short
	
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1) { // 소켓에 주소를 바인딩함
		error_handling("bind() error");
	}

	if (listen(serv_sock, 5) == -1) { // 리스닝 소켓 할당 완료 -> 연결 요청 대기 큐는 5까지 할당
		error_handling("listen() error");
	}

	clnt_adr_sz = sizeof(clnt_adr);

	for (i = 0; i < 5; ++i) { // Iterative Server
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_sz); // 실제 데이터를 전송하는 소켓 디스크립터
		if (clnt_sock == -1) {
			error_handling("accept() error");
		} else {
			printf("Connected client >> %d \n", i + 1);
		}

		while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) { // 연결을 종료하면 0이 되므로 빠져 나갈 것임
			write(clnt_sock, message, str_len); // Echo (클라이언트로부터 온 메시지를 다시 보냄)
		}
		
		close(clnt_sock); // 한 순간에 하나의 클라이언트와 연결되어 에코 서비스를 제공하면 연결을 끊어야 함
	}
	
	close(serv_sock); // 리스닝 소켓 연결 종료	

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
