#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock; // socket(file) descriptor
	char message[30];
	int option, str_len;
	socklen_t optlen, clnt_adr_sz;
	struct sockaddr_in serv_adr, clnt_adr;
	
	if (argc != 2) {
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket() error!");
	}


	optlen = sizeof(option);
	option = TRUE;
	// SO_REUSEADDR 옵션을 통해 1(TRUE)을 전달하면 -> Time Wait 상태를 중지시킨다!
	// Time Wait는 4way handshaking에서 마지막 ACK를 받기 위해 먼저 FIN 메시지를 보낸 곳에서 하는 것이다!
	// 그러나, Time Wait 상태가 계속 ACK 메시지를 받지 못한다면 계속 타이머가 재가동될 것이고
	// 그러면 같은 포트 번호에 소켓을 할당할 수 없는 상태(서버 단에서)가 된다. 이것을 중지시키는 것이다.
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// Time Wait가 켜져 있다면  이를 재실행했을 때 여기서 문제가 발생한다.
	// 클라이언트 포트는 OS가 임의로 할당하지만, 서버는 그렇지 않기 때문이다.
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
		error_handling("bind() error!");

	if (listen(serv_sock, 5) == -1) // 연결 요청 대기 큐 사이즈 5
		error_handling("listen() error!");

	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

	while ((str_len = read(clnt_sock, message, sizeof(message))) != 0) {
		write(clnt_sock, message, str_len);
		write(1, message, str_len);
	}

	close(clnt_sock);
	close(serv_sock);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
