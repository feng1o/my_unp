/*************************************************************************
    > File Name: singlethreadselectserver.c
  > Author: 
  > Mail: 
  > Created Time: Sat 30 Apr 2016 07:21:06 PM CST
 ************************************************************************/

#include<stdio.h>
#include"stdlib.h"
#include"netinet/in.h"
#include"arpa/inet.h"
#include"sys/socket.h"
#include"sys/types.h"

#include"../lib/writen.c"
#include"../lib/wrapunix.c"
#include"../lib/error.c"
#include"string.h"
#define LISTENQ 5
#define MAXLINE 4096
int main()
{
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    int i, maxi, maxfd, sockfd;

    ssize_t n,nready, client[FD_SETSIZE];
    fd_set rset, allset;
    char buf[MAXLINE];

    socklen_t  clilen;
    listenfd = socket(AF_INET, SOCK_STREAM, 0); 

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(6666);
    
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);
    
    maxfd = listenfd;
    maxi = -1;    //index for client[],,
    for(i=0; i<FD_SETSIZE; ++i)
        client[i] = -1;  //available
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    for(; ;)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);

        if(FD_ISSET(listenfd, &rset)) {  //new connect client
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            printf("new connect ip   = %s\n", inet_ntoa(cliaddr.sin_addr));
            printf("new connect port = %d\n", ntohs(cliaddr.sin_port));
            
            for(i=0; i<FD_SETSIZE; ++i){
                if(client[i] < 0){
                    client[i] = connfd;
                    printf("connected socket number =%d\n",i);
                    break;
                }
            }
            if( i == FD_SETSIZE )
                err_quit("too many client .....\n");
            FD_SET(connfd, &rset);
            if(connfd > maxfd)
                maxfd = connfd;
            if(i>maxi)
                maxi = i;
            if(--nready <= 0)
                continue;
        }

        for(i =0; i<=maxi; ++i){
            printf("go to connected socket........\n");
            if((sockfd = client[i]) < 0)
                continue;
            if(FD_ISSET(sockfd, &rset)){
                if( n = Read(sockfd, buf, MAXLINE) == 0 )
                {
                    printf("closing....socket %d\n",sockfd);
                    close(sockfd);
                    FD_CLR(sockfd, &rset);
                    client[i] = -1;
                }
                else{
                    printf("....write back ...\n");
                    Writen(sockfd, buf, n);
                    fputs(buf, stdout);
                }
                if(--nready <= 0 )
                    break;
            }
        }
    }
    return 0;
    
 }
