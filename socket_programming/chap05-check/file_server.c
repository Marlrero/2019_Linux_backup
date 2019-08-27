#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h> // 가장 중요!

void error_handling(char* message); //함수의 원형

int main(int argc, char* argv[]) {
	int serv_sock; // 서버 소켓
	int clnt_sock; // 클라이언트 소켓

	struct sockaddr_in serv_addr; //서버 주소를 담고 있는 구조체 변수
	struct sockaddr_in clnt_addr; //클라이언트 주소를 담고 있는 구조체 변수
	socklen_t clnt_addr_size; //클라이언트 주소의 크기

	char message[] = "Hello World!"; // 보낼 메시지

	if (argc != 2) { // 메인 함수 인자가 2가 아니면
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	//1. int socket(int domain, int typoe, int protocol);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0); // 소켓 생성
	if (serv_sock == -1) { // 소켓 생성 실패 시
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); //서버 소켓 사이즈(4 bytes)
						//만큼 메모리를 할당함
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//2. int bind(int sockfd, struct sockaddr* myaddr, socklen_t addrlen);
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error");
	} // bind 함수호출을 통해 ip 주소와 port 번호를 할당

	//3. int listen(int sockfd, int backlog);
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	} // listen 함수를 호출해 연결 요청을 받아들일 수 있게 함

	clnt_addr_size = sizeof(clnt_addr); //클라이언트 주소 크기 초기화

	//4. int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	  // 연결 요청 수락을 위한 accept 함수 호출 -> 연결요청이 있을 때까지 반환하지 않음
	
	if (clnt_sock == -1) {
		error_handling("accept() error");
	}

	// 여기까지 왔다면 연결요청이 있었다는 뜻!
	write(clnt_sock, message, sizeof(message)); //클라이언트에게 데이터를 전송함
	close(clnt_sock); // 클라이언트 소켓 닫기!
	close(serv_sock); // 서버 소켓 닫기!	

	return 0;
}

void error_handling(char* message) { // 오류가 발생되면 실행되는 함수 정의
	fputs(message, stderr); //표준 오류 출력 스트림에게 메시지를 전달함
	fputc('\n', stderr);
	exit(1);
}
