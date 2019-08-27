#include <stdio.h>
#include <stdlib.h> // memset(), exit()
#include <string.h> // fputs(), fgets(), strcmp()
#include <unistd.h> // read(), write(), close()
#include <arpa/inet.h> // inet_addr(), htons()
#include <sys/socket.h> // socket(), connect()

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sock; // 클라이언트 소켓
	struct sockaddr_in serv_adr; // 서버 주소
	
	if (argc != 3) { // main 함수 인자가 3개가 아니면
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성
	if (sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_adr, 0, sizeof(serv_adr)); // sin_zero 필드까지 0으로 초기화해야 함
	serv_adr.sin_family = AF_INET; // IPv4 주소 체계 사용
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]); // Dotted Decimal Notation -> 32 bits big endian(network byte order) convert
	serv_adr.sin_port = htons(atoi(argv[2])); // host to network short -> port# big endian(network byte order) convert

	if (connect(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1) { // 연결 요청
		error_handling("connect() error");
	} else {
		puts("Connected.......\n");
	}

	int operand_count, write_byte = 0, write_cnt;
	printf("Operand count: ");
	scanf("%d", &operand_count);
	while (write_byte <= sizeof(int)) {
		write_cnt = write(sock, &operand_count, sizeof(int));
		write_byte += write_cnt;
	}

	write_byte = write_cnt = 0;
	int* operand = (int*) malloc(sizeof(int) * operand_count);
	int i;
	for (i = 0; i < operand_count; ++i) {
		printf("Operand %d: ", i + 1);
		scanf("%d", &operand[i]);
	}

	while (write_byte <= sizeof(int) * operand_count) {
		write_cnt = write(sock, operand, sizeof(int) * operand_count);
		write_byte += write_cnt;
	}
	
	getchar();
	write_byte = write_cnt = 0;
	char operator;
	printf("Operator: ");
	scanf("%c", &operator);
	
	while (write_byte <= sizeof(char)) {
		write_cnt = write(sock, &operator, sizeof(char));
		write_byte += write_cnt;
	}

	int result, read_cnt = 0, read_byte = 0;
	while (read_byte <= sizeof(int)) {
		read_cnt = read(sock, &result, sizeof(int));
		read_byte += read_cnt;
	}
	printf("Operation result: %d \n", result);

	free(operand);
	operand = NULL;

	close(sock); // 클라이언트 소켓 종료	

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
