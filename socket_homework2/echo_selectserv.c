#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#include <pthread.h>
#include <semaphore.h>

#define BUF_SIZE 100

void error_handling(char *msg);

void *handle_clnt(void *arg);

//static sem_t sem;
pthread_mutex_t mutex;
fd_set reads;

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set cpy_reads;
	socklen_t adr_sz;

	int fd_max, fd_num, i;
	//char buf[BUF_SIZE];

	pthread_t t_id;

//	sem_init(&sem, 0, 1);
	pthread_mutex_init(&mutex, NULL);

	if (argc != 2) {
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error!");
	
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while (1) {
//		sem_wait(&sem);
		pthread_mutex_lock(&mutex);
		cpy_reads = reads;
//		sem_post(&sem);
		pthread_mutex_unlock(&mutex);


		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;

		if (fd_num == 0)
			continue;

		for (i = 0; i < fd_max + 1; i++) {
			if (FD_ISSET(i, &cpy_reads)) {
				if (i == serv_sock) {
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_adr, &adr_sz);
					
//					sem_wait(&sem);
					pthread_mutex_lock(&mutex);
					FD_SET(clnt_sock, &reads);
					pthread_mutex_unlock(&mutex);
//					sem_post(&sem);					

					if (fd_max + 1 < clnt_sock)
						fd_max = clnt_sock;

					printf("connected client: %d \n", clnt_sock); 
				} else {
					pthread_create(&t_id, NULL, handle_clnt, (void*)&i);
					pthread_detach(t_id);					
				}
			}
		}

	}

//	sem_destroy(&sem);
	pthread_mutex_destroy(&mutex);
	close(serv_sock);
	return 0;
}

void error_handling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void *handle_clnt(void *arg) {
	printf("thread create!! \n");
	int clnt_sock = *((int *)arg);
	int str_len;
	char buf[BUF_SIZE];

	str_len = read(clnt_sock, buf, BUF_SIZE);	
	if (str_len == 0) {
//		sem_wait(&sem);
		pthread_mutex_lock(&mutex);
		FD_CLR(clnt_sock, &reads); 
		pthread_mutex_unlock(&mutex);
//		sem_post(&sem);
		
		close(clnt_sock);
	printf("closed client: %d \n", clnt_sock);
	} else {
		write(clnt_sock, buf, str_len);
	}

	return NULL;
}
