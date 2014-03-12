#include<time.h>
#include<netinet/in.h>
//#include"share.h"
#include<string.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<errno.h>

#define MAXLINE 4098
#define LISTENQ 1024

int main(int argc, char **argv)
{
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  char buff[MAXLINE];
  time_t ticks;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket error\n");
    return -1;
  }
  printf("listenfd=%d\n", listenfd);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(13);

  //include in <sys/socket.h>
  if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
    printf("bind error\n");
    //errno include in <errno.h>
    printf("errno:%d\n", errno);
    return -1;
  }

  if (listen(listenfd, LISTENQ) < 0) {
    printf("listen error\n");
    return -1;
  }

  for ( ; ; ) {
    if (connfd = accept(listenfd, (struct sockaddr*) NULL, NULL) < 0) {
      printf("accept error\n");
      return -1;
    }
    
    ticks = time(NULL);
    //snprintf include in <stdio.h>
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    //strlen include in <string.h>
    if (write(connfd, buff, strlen(buff)) < 0) {
      printf("write error\n");
      return -1;
    }
    close(connfd);
  }
}
