/*************************************************************************
    > File Name: selectshutdownclient.c
  > Author: 
  > Mail: 
  > Created Time: Sat 30 Apr 2016 05:26:32 PM CST
 ************************************************************************/

#include<stdio.h>
#include"stdlib.h"
#include"netinet/in.h"
#include"sys/socket.h"
#include"arpa/inet.h"
#include"unistd.h"
#include"strings.h"
#include"fcntl.h"

#include"../lib/error.c"
#include"../lib/wrapunix.c"
#include"../lib/readn.c"
#include"../lib/writen.c"
#include"sys/types.h"
#include"sys/select.h"
#define MAXLINE  4096

void str_client(FILE *fp, int fd);
int main()
{
    int sockfd;
    struct sockaddr_in  servaddr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.4", &servaddr.sin_addr);

    printf("connect to port =%d \n", ntohs(servaddr.sin_port));
    printf("connect to ip   =%s \n", inet_ntoa(servaddr.sin_addr));

    connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr) );
    str_client(stdin, sockfd);

    
    return 0;
}

void str_client(FILE *fp, int fd)
{
    int maxfds;
    fd_set   rset;
    char buf[MAXLINE];
    
    int stdineof = 0;
    int n;
    FD_ZERO(&rset);
    for(; ;){
        if(stdineof == 0)
            FD_SET(fileno(fp), &rset);
        FD_SET(fd, &rset);
        maxfds = fd > fileno(fp) ? fd + 1 : fileno(fp) + 1 ;
        select(maxfds, &rset, NULL, NULL, NULL);

        if( FD_ISSET(fd, &rset)) {
            if(n = Read(fd, buf, MAXLINE) == 0){
                if(stdineof == 1)
                    return ;
                else
                    err_quit("server terminated prematurely\n");
            }
            Writen(STDOUT_FILENO, buf, n); 
        }
        if(FD_ISSET(fileno(fp), &rset)){
            if(n = Read(fileno(fp), buf, MAXLINE) == 0)
                stdineof = 1;
            shutdown(fd, SHUT_WR);
            FD_CLR(fileno(fp), &rset);
            continue;
        }
        Writen(fd, buf, n);
    }
}
