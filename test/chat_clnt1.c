#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
	int sock;
	char message[BUF_SIZE];
        char send[BUF_SIZE];
	int str_len,recv_len,recv_cnt;

	struct sockaddr_in serv_adr;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));	

        if (connect(sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");

	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE - 1, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;
                
                send[0] = (char)strlen(message);//형변환
                strcpy(&send[1], message);//메시지 값 복사 send에
		printf("%s",send);
                write(sock, &send[0], 1);
		write(sock,&send[1],sizeof(send)-1);
		printf("%d \n", (int)send[0]);

                recv_len = 0;
                recv_cnt = 0;

		read(sock, &str_len, 1);
		while (recv_len < str_len)
                {
                     recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
                     recv_len = recv_len + recv_cnt;

          

                }
             
               message[recv_len] = 0;//문자열 끝 인식
               printf("-> %s", message);

	}
	close(sock);
	return 0;
}

void error_handling(char *message) {
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}

