#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <stdarg.h>

#define BUF_SIZE 1024

void error_handling(char* message);
void init_socket(int* listen_sock, int* data_sock, int port, struct sockaddr_in* serv_adr, struct sockaddr_in* clnt_adr);
int read_and_write(int read_sock, int write_sock);
int is_file_send(char* message);
void read_file(int read_sock);
void write_file(int write_sock);
void close_socket(int args, ...);

int main(int argc, char* argv[]) {
	int listen_sock1, listen_sock2, data_sock1, data_sock2;
	struct sockaddr_in serv_adr1, serv_adr2, clnt_adr1, clnt_adr2;
		
	if (argc != 3) {
		printf("Usage: %s <port> <port> \n", argv[0]);
		exit(1);
	}

	init_socket(&listen_sock1, &data_sock1, atoi(argv[1]), &serv_adr1, &clnt_adr1);
	init_socket(&listen_sock2, &data_sock2, atoi(argv[2]), &serv_adr2, &clnt_adr2);

	int is_exit;
	while (1) {
		is_exit = read_and_write(data_sock1, data_sock2);
		if (is_exit) break;
		read_and_write(data_sock2, data_sock1);
		if (is_exit) break;
	}

	close_socket(4, listen_sock1, listen_sock2, data_sock1, data_sock2);

	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void init_socket(int* listen_sock, int* data_sock, int port, struct sockaddr_in* serv_adr, struct sockaddr_in* clnt_adr) {
	*listen_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (*listen_sock == -1)
		error_handling("socket() error!");

	memset(serv_adr, 0, sizeof(*serv_adr));
	serv_adr->sin_family = PF_INET;
	serv_adr->sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr->sin_port = htons(port);

	if (bind(*listen_sock, (struct sockaddr*) serv_adr, sizeof(*serv_adr)) == -1)
		error_handling("bind() error!");

	if (listen(*listen_sock, 5) == -1)
		error_handling("listen() error!");

	socklen_t clnt_adr_size = sizeof(*clnt_adr);
	*data_sock = accept(*listen_sock, (struct sockaddr*) clnt_adr, &clnt_adr_size);
	if (*data_sock == -1)
		error_handling("accept() error!");

	printf("client connected! >> fd: %d \n", *data_sock);
}

int read_and_write(int read_sock, int write_sock) {
	int str_len = 0, recv_len = 0, recv_cnt;
	char buffer[BUF_SIZE];
	char send_message[BUF_SIZE];

	read(read_sock, &str_len, 1); // 메시지 길이
	printf("클라이언트에서 읽어야 하는 바이트: %d \n", str_len);
	while (recv_len < str_len) {
		printf("recv_len >> %d \n", recv_len);
		recv_cnt = read(read_sock, &buffer[recv_len], BUF_SIZE - 1);
		recv_len += recv_cnt;
	}
	buffer[recv_len] = 0; // 널 문자	
	printf("클라이언트%d에서 받음 >> %s", read_sock, buffer);

	if (!strcmp(buffer, "quit\n")) {
		char* end_message = "partener disconnected\n";
		printf("클라이언트%d 종료\n", read_sock);
		
		send_message[0] = (char)strlen(end_message);
		strcpy(send_message + 1, end_message);
		write(write_sock, send_message, (int)send_message[0] + 1);
		return 1;
	}

	send_message[0] = (char)str_len;
	strcpy(send_message + 1, buffer);	
	write(write_sock, send_message, strlen(send_message));
	printf("클라이언트%d에게 보냄 >> %s", write_sock, buffer);

	if (is_file_send(buffer)) {
		printf("서버가 파일 전송 모드로 전환\n");
		read_file(read_sock);
		write_file(write_sock);
		printf("서버가 파일 전송 모드를 종료\n");
	}

	return 0;
}

int is_file_send(char* message) {
	if (strlen(message) < 5)
		return 0;
	else
		return message[0] == 'f' && message[1] == 'i' &&
	       	       message[2] == 'l' && message[3] == 'e' && message[4] == ' ';
}

void read_file(int read_sock) {
	FILE* fp = fopen("read.dat", "wb");
	char buffer[BUF_SIZE];
	int read_cnt, recv_len = 0;

	while ((read_cnt = read(read_sock, buffer, BUF_SIZE)) == BUF_SIZE) {
		printf("read_cnt >> %d \n", read_cnt);
		fwrite((void*)buffer, 1, read_cnt, fp);
	}
	fwrite((void*)buffer, 1, read_cnt, fp);

	printf("클라이언트%d로부터 데이터를 모두 읽음!\n", read_sock);
	fclose(fp);
}

void write_file(int write_sock) {
	FILE* fp = fopen("read.dat", "rb");
	char buffer[BUF_SIZE];
	int read_cnt;

	while (1) {
		read_cnt = fread((void*)buffer, 1, BUF_SIZE, fp);
		if (read_cnt < BUF_SIZE) {
			write(write_sock, buffer, read_cnt);
			break;
		}
		write(write_sock, buffer, BUF_SIZE);
	}

	printf("클라이언트%d에게 파일을 보냈음!\n", write_sock);
	fclose(fp);
}

void close_socket(int args, ...) {
	va_list arg_list;
	int i;

	va_start(arg_list, args); // 가변 인자 리스트 초기화
	for (i = 0; i < args; ++i) {
		int sd = va_arg(arg_list, int); // int 만큼 이동
		close(sd);
	}
	va_end(arg_list);
}
