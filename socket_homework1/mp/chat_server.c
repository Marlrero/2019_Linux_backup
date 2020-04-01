#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define CLNT_NUM 10

void error_handling(char* message);
void read_childproc(int sig);

int main(int argc, char* argv[]) {
	int serv_sock;
	int clnt_sock[CLNT_NUM];
	struct sockaddr_in serv_adr, clnt_adr;
	
	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;

	char message[BUF_SIZE];

	int fds[2];
	pipe(fds); // ipc

	if (argc != 2) {
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error!");
	
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	int msg_len; // 메시지를 읽기 위한 길이
	int recv_len, recv_cnt;
	int clnt_cnt = -1; // 클라이언트의 갯수(0이면 1개)
	int i;

	while (1) {
		adr_sz = sizeof(clnt_adr);
		clnt_sock[++clnt_cnt] = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
		if (clnt_sock[clnt_cnt] == -1) { // 에러
			clnt_cnt--;
			continue;
		} else {
			printf("새로운 클라이언트 연결중...\n");
		}

		pid = fork();
		if (pid == 0) { // 자식 프로세스 실행 영역
			close(serv_sock); // 자식 프로세스는 리스닝 소켓 필요 없음
			
			// 서버는 클라이언트로부터 메시지 크기를 먼저 읽어 들임
			read(clnt_sock[clnt_cnt], &msg_len, 1);
		
			// 그리고 클라이언트로부터 메시지 크기만큼 메시지를 읽어 들임
			recv_len = 0;
			while (recv_len < msg_len) {
				recv_cnt = read(clnt_sock[clnt_cnt], &message[recv_len], BUF_SIZE - 1);
				recv_len += recv_cnt;
			}
			message[recv_len] = 0;

			// file인지 검사해야 하는 로직

			// 부모 프로세스에서 데이터를 보내야 함 -> ipc
			/*write(fds[1], message, sizeof(message));

			close(clnt_sock[clnt_cnt--]);
			printf("클라이언트 종료!");*/

			pid = fork();
			if (pid == 0) { // 자식 프로세스 실행 영역
				// 모든 클라이언트에게 보낸다!
				char buf[BUF_SIZE];
				for (i = 0; i < clnt_cnt; ++i) {
					msg_len = strlen(message);
					buf[0] = (char)msg_len;
					strcpy(&buf[1], message);
					printf("client >> %s", buf);
					write(clnt_sock[i], buf, strlen(buf));
				}
				return 0;
			} else { // 부모 프로세스 실행 영역

			}

			return 0;
		} else { // 부모 프로세스 영역
			//close(clnt_sock);
		}
	}
	
	close(serv_sock);
	return 0;	
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_childproc(int sig) {
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
}
