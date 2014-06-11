#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/select.h>

#define MAXLINE 4098
#define SERV_PORT 9877     //which port the client want to connect
#define MAX_LINKE 1        //maximun link the client will create

void str_cli(FILE *fp, int sockfd)
{
  char sendline[MAXLINE], recvline[MAXLINE];
/*
  while (fgets(sendline, MAXLINE, fp) != NULL) {
    write(sockfd, sendline, strlen(sendline));
    if (read(sockfd, recvline, MAXLINE) == 0) {
      printf("str_cli: server terminated prematurely\n");
      exit(0);
    }
    fputs(recvline, stdout);
  }
*/
  //use select()
  int maxfdp1;
  fd_set rset;
  FD_ZERO(&rset);
  for (;;) {
    //for fget()
    FD_SET(fileno(fp), &rset);
    //fot socket
    FD_SET(sockfd, &rset);
    maxfdp1 = (fileno(fp) > sockfd? fileno(fp):sockfd) + 1;
    select(maxfdp1, &rset, NULL, NULL, NULL);

    //socket is readable
    if (FD_ISSET(sockfd, &rset)) {
      if (read(sockfd, recvline, MAXLINE) == 0) {
        printf("str_cli: server terminated prematurely\n");
        exit(0);
      }
      fputs(recvline, stdout);
    }
    //input is readable
    if (FD_ISSET(fileno(fp), &rset)) {
      if (fgets(sendline, MAXLINE, fp) == NULL) {
        shutdown(sockfd, SHUT_WR);
        FD_CLR(fileno(fp), &rset);
        continue;
      }
      write(sockfd, sendline, MAXLINE);
    }
  }
}

int main(int argc, char **argv)
{
  int sockfd[MAX_LINKE], n;
//  char recvline[MAXLINE + 1];
  struct sockaddr_in servaddr;

  if (argc != 2) {
    printf("usage: <IPaddress>\n");
    return -1;
  }

int i;
for (i = 0; i < MAX_LINKE; i++) {
  if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket error\n");
    return -1;
  }
  
  //include in string.h
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  //inet_pton include in <arpa/inet.h>
  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    printf("inet_pton error\n");
    return -1;
  }

  if (connect(sockfd[i], (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
    printf("connect error\n");
    return -1;
  }
  
  //read include in <unistd.h>
/*  while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
    recvline[n] = 0;
    if (fputs(recvline, stdout) == EOF) {
      printf("fputs error\n");
      return -1;
    }
  }
  if (n < 0) {
    printf("read error\n");
    return -1;
  }
*/
}
  str_cli(stdin, sockfd[0]);

  //include in <stdlib.h>
  exit(0);
}
