#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int serv_sd, clnt_sd; // socket descriptor
	FILE* fp; // file pointer
	char buf[BUF_SIZE]; // buffer
	int read_cnt; // read count

	struct sockaddr_in serv_adr, clnt_adr; // server, client address
	socklen_t clnt_adr_sz; // client address size

	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("file_server.c", "rb"); // open file (read binary mode)
	serv_sd = socket(PF_INET, SOCK_STREAM, 0); // create server socket (TCP)
	
	memset(&serv_adr, 0, sizeof(serv_adr)); // init sin_zero field
	serv_adr.sin_family = AF_INET; // IPv4 format
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // server ip
	serv_adr.sin_port = htons(atoi(argv[1])); // server port

	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)); // bind address
	listen(serv_sd, 5); // waiting request queue 5
	
	clnt_adr_sz = sizeof(clnt_adr);
	clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz); // data socket
	
	while (1) {
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp); // fp -> buf (read)
		if (read_cnt < BUF_SIZE) {
			write(clnt_sd, buf, read_cnt); // transmit client
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}

	shutdown(clnt_sd, SHUT_WR); // SHUT_WR: output stream half-close (버퍼에 남겨진 것은 보내지고, 더 이상 보낼 수 없다!)
	read(clnt_sd, buf, BUF_SIZE); // 앞서, 출력 스트림만 닫았으므로 입력 스트림은 살아있다!
	printf("Message from client: %s \n", buf);

	fclose(fp);
	close(clnt_sd);
	close(serv_sd);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
