#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message);

int main(int argc, char* argv[]) {
	int sd; // socket descriptor
	struct sockaddr_in serv_adr; // server address

	FILE* fp; // file pointer
	char buf[BUF_SIZE]; // buffer
	int read_cnt; // read count
	
	if (argc != 3) {
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.dat", "wb"); // open file (write binary mode)

	sd = socket(PF_INET, SOCK_STREAM, 0); // create socket (TCP)

	memset(&serv_adr, 0, sizeof(serv_adr)); // init sin_zero field
	serv_adr.sin_family = AF_INET; // IPv4 format
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]); // server ip address
	serv_adr.sin_port = htons(atoi(argv[2])); // server port number

	connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0) { // read_cnt == 0라면 더 이상 읽을 것이 없다는 의미
		fwrite((void*)buf, 1, read_cnt, fp); // write file
	}

	puts("Received file data!");
	write(sd, "Thank You", 10); // 서버는 half-close (출력 스트림)했으므로 이를 충분히 받을 수 있다!

	fclose(fp);
	close(sd);
	return 0;
}

void error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
