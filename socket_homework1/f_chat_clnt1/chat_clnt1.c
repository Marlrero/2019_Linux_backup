#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message);
int is_file_send(char* message, char* file_name);
int exist_file(char* fime_name);
void read_file(int read_sock, char* file_name);
void write_file(int write_sock, char* flie_name);

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in serv_adr;
	char buffer[BUF_SIZE];
	char send_message[BUF_SIZE];
	int str_len = 0, recv_len, recv_cnt;

	char file_name[100];

	if (argc != 3) {
		printf("Usage: %s <ip> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error!");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!");

	while (1) {
		fputs("input >> ", stdout);
		fgets(buffer, BUF_SIZE, stdin);

		send_message[0] = (char)strlen(buffer); // 메시지 길이
		
		strcpy(&send_message[1], buffer);

		if (!strcmp(buffer, "quit\n")) { // 종료 조건
			write(sock, send_message, (int)send_message[0] + 1);
			break;
		}

		if (is_file_send(buffer, file_name)) {
			if (file_name[0] != '\0' && exist_file(file_name)) {
				write(sock, send_message, (int)send_message[0] + 1);
				write_file(sock, file_name);
			} else {
				send_message[0] = (char)1;
				strcpy(&send_message[1], " ");
				write(sock, send_message, (int)send_message[0] + 1);
			}
		} else {
			write(sock, send_message, (int)send_message[0] + 1);
		}

		read(sock, &str_len, 1); // 메시지 길이

		recv_len = 0;
		while (recv_len < str_len) {
			recv_cnt = read(sock, &buffer[recv_len], BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		buffer[recv_len] = 0; // 널 문자

		if (is_file_send(buffer, file_name)) {
			read_file(sock, file_name);
		} else { 
			printf("-> %s", buffer);
		}
	}

	close(sock);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int is_file_send(char* message, char* file_name) {
	int i;

	if (strlen(message) > 5 &&
	    message[0] == 'f' && message[1] == 'i' &&
	    message[2] == 'l' && message[3] == 'e' && message[4] == ' ') {
		if (message[5] != '\0') {
			strcpy(file_name, message + 5);
			for (i = 0; file_name[i] != '\0'; i++) { // \n을 지워야 함!
				if (file_name[i] == '\n')
					file_name[i] = '\0';
			}
			return 1;
		} else {
			file_name[0] = '\0';
			return 0;
		}
	} else {
		file_name[0] = '\0';
		return 0;
	}
}

int exist_file(char* file_name) {
	FILE* fp = fopen(file_name, "rb");
	if (fp == NULL) {
		puts("File not found!");
		return 0;
	} else {
		fclose(fp);
		return 1;
	}
}

void read_file(int read_sock, char* file_name) {
	FILE* fp = fopen(file_name, "wb");
	char buffer[BUF_SIZE];
	int read_cnt;

	while((read_cnt = read(read_sock, buffer, BUF_SIZE)) == BUF_SIZE) {
		fwrite((void*)buffer, 1, read_cnt, fp);
	}
	fwrite((void*)buffer, 1, read_cnt, fp);
	
	printf(">>> file received: %s \n", file_name);
	fclose(fp);
}

void write_file(int write_sock, char* file_name) {
	FILE* fp = fopen(file_name, "rb");
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

	printf(">>> %s sent! \n", file_name);
	fclose(fp);
}
