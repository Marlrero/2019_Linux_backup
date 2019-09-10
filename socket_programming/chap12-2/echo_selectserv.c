#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(char* buf);

int main(int argc, char* argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;

	char buf[BUF_SIZE];
	int str_len;

	struct timeval timeout;
	fd_set reads, cpy_reads;
	int fd_max, fd_num, i;
	
	if (argc != 2) {
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error!");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	FD_ZERO(&reads); // 모든 비트를 0으로 초기화
	FD_SET(serv_sock, &reads); // 서버 소켓을 관찰 대상에 추가
	fd_max = serv_sock;
	printf("init fd_max: %d \n", fd_max);

	while (1) {
		cpy_reads = reads; // select 함수가 실행되면 변화가 생긴 디스크립터 위치를 제외한 나머지가 모두 0으로 초기화되므로 복사해놔야 함
		timeout.tv_sec = 5; // 타임아웃을 5초로 등록
		timeout.tv_usec = 5000; // 이 초기화도 마찬가지로, select 함수가 실행되면 남은 시간으로 초기화되기 때문에 매번 해야 함
		
		// fd_max + 1 -> 파일 디스크립터는 0부터 시작하므로 최대 개수는 1을 더해야 한다!
		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1) // 오류 발생
			break;
		if (fd_num == 0) // 타임 아웃
			continue;

		// 아랫 부분이 실행된다는 의미는 수신 여부가 있었다는 의미임
		for (i = 0; i < fd_max + 1; ++i) { // 변화가 생긴 파일 디스크립터 모두에 대해
			if (FD_ISSET(i, &cpy_reads)) { // i에 대한 디스크립터 정보가 있는가?
				if (i == serv_sock) { // 이 i가 서버 디스크립터 인가? -> 연결 요청
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz); // 클라이언트와 연결된 소켓을 생성
					FD_SET(clnt_sock, &reads); // 클라이언트와 연결된 소켓을 관찰 대상으로 지정함
					
					if (fd_max < clnt_sock) // 클라이언트 소켓 디스크립터가 fd_max보다 크면
						fd_max = clnt_sock; // 최대 갯수를 clnt_sock으로 잡아야 함

					printf("connected client: %d \n", clnt_sock);
				} else { // i가 서버 디스크립터가 아니면 -> 클라이언트에서 데이터를 보낸 것임
					str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0) { // 종료 요청을 보낸 것임
						FD_CLR(i, &reads); // 이 소켓 디스크립터를 관찰 대상에서 삭제
						close(i); // 연결 종료
						printf("closed clinet: %d \n", i);
					} else {
						write(i, buf, str_len); // echo
					}
				}
			}
		}
	}
	
	close(serv_sock);
	return 0;
}

void error_handling(char* buf) {
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
