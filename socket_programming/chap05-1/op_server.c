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

	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_sz); // 실제 데이터를 전송하는 소켓 디스크립터
	if (clnt_sock == -1) {
		error_handling("accept() error");
	}
	
	int read_byte = 0, read_cnt = 0;
	int operand_count = 0;
	while (read_byte <= sizeof(int)) {
		read_cnt = read(clnt_sock, &operand_count, sizeof(int));
		read_byte += read_cnt;
	}
	printf("debug operand_count >> %d\n", operand_count);	

	read_byte = read_cnt = 0;
	int* operand = (int*) malloc(sizeof(int) * operand_count);
	while (read_byte <= sizeof(int) * operand_count) {
		read_cnt = read(clnt_sock, operand, sizeof(int) * operand_count);
		read_byte += read_cnt;
	}
	printf("debug operand >> \n");
	for (i = 0; i < operand_count; ++i) {
		printf("%d \n", operand[i]);
	}

	read_byte = read_cnt = 0;
	char operator;
	while (read_byte <= sizeof(char)) {
		read_cnt = read(clnt_sock, &operator, sizeof(char));
		read_byte += read_cnt;
	}
	printf("debug operator >> %c \n", operator);

	int result = operand[0];
	for (i = 1; i < operand_count; ++i) {
		switch (operator) {
			case '+': result += operand[i]; break;
			case '-': result -= operand[i]; break;
			case '*': result *= operand[i]; break;
			case '/': result /= operand[i]; break;
		}
	}
	printf("debug result >> %d \n", result);

	int write_byte = 0, write_cnt = 0;
	while (write_byte <= sizeof(int)) {
		write_cnt = write(clnt_sock, &result, sizeof(int));
		write_byte += write_cnt;
	}

	free(operand);
	operand = NULL;

	close(clnt_sock);
	close(serv_sock); // 리스닝 소켓 연결 종료	

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
