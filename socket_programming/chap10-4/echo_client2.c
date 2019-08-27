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
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
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

	while (1) { // 여기서부터 중요!
		fputs("Input message(Q to quit): ", stdout); // 콘솔 출력
		fgets(message, BUF_SIZE, stdin); // 콘솔 입력으로부터 입력받음

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) { // q나 Q를 누르고 엔터를 치면
			// strcmp는 같으면 0을 반환한다
			break;
		}
		
		// 에코 클라이언트의 문제를 수정한 코드임
		str_len = write(sock, message, strlen(message));		

		recv_len = 0;
		while (recv_len < str_len) { // str_len 바이트를 읽어들일 때까지 read를 계속 호출함
			recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
			if (recv_cnt == -1) {
				error_handling("read() error");
			}
			recv_len += recv_cnt; // read에서 읽어들인 만큼 recv_len에 더함
		}
		message[recv_len] = 0; // 마지막에 널 문자를 삽입함
		printf("Message from server: %s", message);
	}

	close(sock); // 클라이언트 소켓 종료	

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
