#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30

void error_handling(char* message);
void urg_handler(int signo); // 이벤트 헨들러

int acpt_sock;
int recv_sock;

int main(int argc, char* argv[]) {
	struct sockaddr_in recv_adr, serv_adr;
	int str_len, state;
	socklen_t serv_adr_sz;
	struct sigaction act;
	char buf[BUF_SIZE];

	if (argc != 2) {
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}
	
	// 긴급 데이터 수신 시 처리할 이벤트 핸들러 등록
	act.sa_handler = urg_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&recv_adr, 0, sizeof(recv_adr));
	recv_adr.sin_family = AF_INET;
	recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	recv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
		error_handling("bind() error!");
	listen(acpt_sock, 5);

	serv_adr_sz = sizeof(serv_adr);
	recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

	fcntl(recv_sock, F_SETOWN, getpid());
	// 파일 디스크립터 컨트롤에 사용하는 함수
	// 파일 디스크립터 recv_sock이 가리키는 소켓에 의해 발생하는 SIGURG 시그널을
	// 처리하는 프로세스를 getpid 함수가 반환하는 ID의 프로세스로 변경시키겠다
	// -> F_SETOWN(소켓 소유자)를 getpid 함수가 반환하는 프로세스 id로 변경!
	// SIGURG 시그널 핸들링 시, 반드시 시그널을 처리할 프로세스를 함께 지정해야 함

	state = sigaction(SIGURG, &act, 0);

	while ((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0) {
		if (str_len == -1)
			continue;
		buf[str_len] = 0;
		puts(buf);
	}
	
	close(recv_sock);
	close(acpt_sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void urg_handler(int signo) {
	int str_len;
	char buf[BUF_SIZE];
	str_len = recv(recv_sock, buf, sizeof(buf) -1, MSG_OOB);
	buf[str_len] = 0;
	printf("Urgent message: %s \n", buf);
}
