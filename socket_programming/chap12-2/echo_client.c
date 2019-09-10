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
	int str_len;
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
		
		// 아래 코드는 잘못된 코드임!!
		/*
			read, write 함수가 호출될 때마다 문자열 단위로 실제 입출력이 이뤄진다는 잘못된 가정이 존재함
			TCP는 데이터 경계(data boundary)가 없다고 했다!
			OS가 어떤 이유에 의해 단편화해서 보내면 어떻게 할 것인가?
			모든 조각이 전송되지 않았더라도 read 함수를 호출할 수도 있지 않은가?
			-> 오류 발생 확률이 있는 코드이므로, 다음 장에서 이를 고칠 것임
		*/
		write(sock, message, strlen(message)); // 서버 소켓에게 메시지를 전송함
		str_len = read(sock, message, BUF_SIZE - 1); // 서버 소켓으로부터 메시지를 읽는다(Echo)
		message[str_len] = 0; // 마지막 부분을 널 문자로 채운다.
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
