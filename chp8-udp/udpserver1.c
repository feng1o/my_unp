/*************************************************************************
    > File Name: udpserver1.c
  > Author: 
  > Mail: 
  > Created Time: Tue 03 May 2016 07:31:10 PM CST
 ************************************************************************/

#include<stdio.h>
#include"sys/socket.h"
#include"arpa/inet.h"
#include"netinet/in.h"
#include"stdlib.h"
#include"fcntl.h"
#include"unistd.h"
#include"string.h"
#include"errno.h"

#define MAXLINE 4096

void dgram_echo(int sockfd, struct sockaddr* cliaddr, socklen_t clilen);

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(6666);

    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    
    dgram_echo(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr));

    return 0;
}


void dgram_echo(int sockfd, struct sockaddr* cliaddr, socklen_t clilen){
    struct sockaddr_in *cliaddr2 = (struct sockaddr_in*) cliaddr;
    printf("client addr is = %s\n", inet_ntoa(cliaddr2->sin_addr));
    printf("client port is = %d\n", ntohs(cliaddr2->sin_port));
    int n;
    socklen_t len;
    char buf[MAXLINE];
    for(; ;)
    {
        len = clilen;
        n = recvfrom(sockfd, buf, MAXLINE, 0, cliaddr, &len );
    struct sockaddr_in *cliaddr2 = (struct sockaddr_in*) cliaddr;
    printf("client addr is = %s\n", inet_ntoa(cliaddr2->sin_addr));
    printf("client port is = %d\n", ntohs(cliaddr2->sin_port));
        if(n<0 && errno == EINTR)
            continue;
        else if(n<0)
            perror("xxxxxxxxxxx");
        
        sendto(sockfd, buf, n, 0, cliaddr, len);

    }
}
