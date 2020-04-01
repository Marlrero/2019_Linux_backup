#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#include <sys/epoll.h>

#define BUF_SIZE 100

#define EPOLL_SIZE 50

void error_handling(char *msg);

int main(int argc, char *argv[]) {
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
//	struct timeval timeout;
//	fd_set reads, cpy_reads;
	socklen_t adr_sz;

	//int fd_max, str_len, fd_num, i;
	int str_len, i;
	char buf[BUF_SIZE];

	struct epoll_event *ep_events; // epoll_wait result
	struct epoll_event event; // epoll_ctl
	int epfd, event_cnt; // epoll_create, epoll_wait return

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

//	FD_ZERO(&reads);
//	FD_SET(serv_sock, &reads);
//	fd_max = serv_sock;

	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
	
	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while (1) {
//		cpy_reads = reads;
//		timeout.tv_sec = 5;
//		timeout.tv_usec = 5000;

//		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
//			break;

//		if (fd_num == 0)
//			continue;

		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);

		if (event_cnt == -1) {
			puts("epoll_wait() error!");
			break;
		}

//		for (i = 0; i < fd_max + 1; i++) {
//			if (FD_ISSET(i, &cpy_reads)) {

		for (i = 0; i < event_cnt; i++) {
			
			//	if (i == serv_sock) {
				if (ep_events[i].data.fd == serv_sock) {
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_adr, &adr_sz);
					//FD_SET(clnt_sock, &reads);

					//if (fd_max + 1 < clnt_sock)
					//	fd_max = clnt_sock;

					event.events = EPOLLIN;
					event.data.fd = clnt_sock;
					epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);

					printf("connected client: %d \n", clnt_sock); 
				} else {
					//str_len = read(i, buf, BUF_SIZE);
					str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
					if (str_len == 0) {
					//	FD_CLR(i, &reads);
					//	close(i);
						epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
						close(ep_events[i].data.fd);
						printf("closed client: %d \n", /*i*/ ep_events[i].data.fd);
 					} else {
					//	write(i, buf, str_len); // echo
						write(ep_events[i].data.fd, buf, str_len);
					}
				}
			}
//		}

	}

	close(serv_sock);
	return 0;
}

void error_handling(char *msg) {
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
