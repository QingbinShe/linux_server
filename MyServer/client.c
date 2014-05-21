#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXLINE 4098
#define SERV_PORT 9877

void str_cli(FILE *fp, int sockfd)
{
  char sendline[MAXLINE], recvline[MAXLINE];
  while (fgets(sendline, MAXLINE, fp) != NULL) {
    write(sockfd, sendline, strlen(sendline));
    if (read(sockfd, recvline, MAXLINE) == 0) {
      printf("str_cli: server terminated prematurely");
      exit(0);
    }
    fputs(recvline, stdout);
  }
}

int main(int argc, char **argv)
{
  int sockfd, n;
//  char recvline[MAXLINE + 1];
  struct sockaddr_in servaddr;

  if (argc != 2) {
    printf("usage: <IPaddress>\n");
    return -1;
  }
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

  if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
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
  str_cli(stdin, sockfd);

  //include in <stdlib.h>
  exit(0);
}
