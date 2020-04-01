#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
	int serv_sock1, serv_sock2, clnt_sock1, clnt_sock2;
	char message[BUF_SIZE];
    char send[BUF_SIZE];
	int str_len = 0, i;
    int recv_cnt, recv_len;
	
	struct sockaddr_in serv_adr1, serv_adr2, clnt_adr1, clnt_adr2;
	socklen_t clnt1_adr_sz,clnt2_adr_sz;
	
	if(argc!=3) {
		printf("Usage : %s <port> <port>\n", argv[0]);
		exit(1);
	}
	serv_sock1 = socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sock1==-1)
		error_handling("socket() error");

	memset(&serv_adr1, 0, sizeof(serv_adr1));
	serv_adr1.sin_family=AF_INET;
	serv_adr1.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr1.sin_port=htons(atoi(argv[1]));

    if (bind(serv_sock1,(struct sockaddr*)&serv_adr1, sizeof(serv_adr1))==-1)
		error_handling("bind() error");
	
	if(listen(serv_sock1, 1) == -1)
		error_handling("listen() error");

	clnt1_adr_sz = sizeof(clnt_adr1);
        clnt_sock1=accept(serv_sock1,(struct sockaddr*)&clnt_adr1, &clnt1_adr_sz);
        
        if (clnt_sock1 == -1)
			error_handling("accept() error");
                
	else
			printf("Connected client %d \n", clnt_sock1);
        
	serv_sock2 = socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sock2==-1)
		error_handling("socket() error");

	memset(&serv_adr2, 0, sizeof(serv_adr2));
	serv_adr2.sin_family=AF_INET;
	serv_adr2.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr2.sin_port=htons(atoi(argv[2]));

    if (bind(serv_sock2, (struct sockaddr*)&serv_adr2, sizeof(serv_adr2))==-1)
		error_handling("bind() error");

        if (listen(serv_sock2, 1) == -1)
		error_handling("listen() error");

	clnt2_adr_sz = sizeof(clnt_adr2);
        clnt_sock2=accept(serv_sock2,(struct sockaddr*)&clnt_adr2, &clnt2_adr_sz);


        if (clnt_sock2 == -1)
			error_handling("accept() error");
                
		else
			printf("Connected client %d \n", clnt_sock2);

   
	while (1) 
        {
            recv_len = 0;
            recv_cnt = 0;
		
		printf("while... \n");
		read(clnt_sock1, &str_len, 1);

                printf("%d", str_len);

		while (recv_len < str_len)
                {
                     recv_cnt = read(clnt_sock1, &message[recv_len], BUF_SIZE - 1);
                     recv_len = recv_len + recv_cnt;
                }
             
                 message[recv_len] = 0;//문자열 끝 인식

             send[0] = (char)str_len;//형변환
             strcpy(send + 1, message);//메시지 값 복사 send에
             write(clnt_sock2, send, (int)send[0] + 1);

            recv_len = 0;
            recv_cnt = 0;

		read(clnt_sock2, &str_len, 1);
		while (recv_len < str_len)
                {
                     recv_cnt = read(clnt_sock2, &message[recv_len], BUF_SIZE - 1);
                     recv_len = recv_len + recv_cnt;

      
                }

          message[recv_len] = 0;//문자열 끝 인식          

               send[0] = (char)str_len;//형변환
           strcpy(send + 1, message);//메시지 값 복사 send에
                     write(clnt_sock1, send, (int)send[0] + 1);

		
	}
	close(serv_sock1);
	close(serv_sock2);
        close(clnt_sock1);
        close(clnt_sock2);
	return 0;
}

void error_handling(char *message) {
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}
