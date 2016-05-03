/*************************************************************************
    > File Name: udpclient1.c
  > Author: 
  > Mail: 
  > Created Time: Tue 03 May 2016 08:03:58 PM CST
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

void dgram_cli(FILE* fp,int sockfd, struct sockaddr* cliaddr, socklen_t clilen);

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.5", &servaddr.sin_addr);
    
    dgram_cli(stdin, sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    return 0;
}


void dgram_cli(FILE *fp,int sockfd, struct sockaddr* servaddr, socklen_t servlen){
    int n;
    socklen_t len;
    char recvline[MAXLINE+1];
    char sendline[MAXLINE];
    
    //verify server reply ,,,use addr port
    struct sockaddr *preply_addr = (struct sockaddr*) malloc(servlen);
    struct sockaddr_in replyaddr ;

    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
        sendto(sockfd, sendline, strlen(sendline), 0,  servaddr, servlen);
        
        len = servlen;
        n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr,  &len);
        if(len != servlen ||  memcmp(servaddr, preply_addr, len) != 0)
        {
            //replyaddr = (struct sockaddr_in)(*preply_addr); 
            //printf("reply from %s (ignored) \n", inet_ntoa(replyaddr.sin_addr));
            continue;
        }
        recvline[n] = '\0';

        fputs(recvline, stdout);
    }
}
