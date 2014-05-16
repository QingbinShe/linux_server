#include<time.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>

#define MAXLINE 4098
#define LISTENQ 1024

void str_echo(int sockfd)
{
  ssize_t n;
  char buf[MAXLINE];

again:
  while ( (n = read(sockfd, buf, MAXLINE)) > 0 )
    write(sockfd, buf, n);
  if (n < 0 && errno == EINTR)
    goto again;
  else if (n < 0) {
    printf("str_echo: read error");
    exit(0);
  }
}

int main(int argc, char **argv)
{
  int listenfd, connfd;
  socklen_t len;
  struct sockaddr_in servaddr, cliaddr;
//  char buff[MAXLINE];
//  time_t ticks;
  pid_t childpid;

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
    len = sizeof(cliaddr);
    if (connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &len) < 0) {
      printf("accept error\n");
      return -1;
    }

    //inet_ntop include in <arpa/inet.h>
/*    if (inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)) == NULL)
      exit(0);
    printf("connection from %s, port %d\n", buff, ntohs(cliaddr.sin_port));
    
    ticks = time(NULL);
    //snprintf include in <stdio.h>
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    //strlen include in <string.h>
    if (write(connfd, buff, strlen(buff)) < 0) {
      printf("write error\n");
      return -1;
    }
*/
    if (childpid = fork() == 0) {
      close(listenfd);
      str_echo(connfd);
      exit(0);
    }
    close(connfd);
  }
}
