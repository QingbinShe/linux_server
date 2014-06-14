#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>

#define MAXLINE 4098
#define SERV_PORT 9877

void dg_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, socklen_t servlen)
{
  int n;
  char sendline[MAXLINE], recvline[MAXLINE];
  socklen_t len;
  struct sockaddr *preply_addr;

  preply_addr = malloc(servlen);

  while(fgets(sendline, MAXLINE, fp) != NULL) {
    sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
    
    len = servlen;
    n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
    //to check if the packet is replied to this client
    if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) {
      //printf("reply from %s (ignore)\n", inet_ntop(...........));
      continue;
    }

    recvline[n] = 0;
    fputs(recvline, stdout);
  }
}

int main(int argc, char **argv)
{
  int sockfd;
  struct sockaddr_in servaddr;

  if (argc != 2) {
    printf("usage: udpcli <IPaddress>\n");
    exit(0);
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  dg_cli(stdin, sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
  exit(0);
}
