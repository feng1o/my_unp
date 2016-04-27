#include"sys/socket.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"unistd.h"
#include"netinet/in.h"
#include"../lib/unp.h"

#define def_SERV_PORT 6666
#define MAXLINE 4096

void str_echo(int );
int main()
{
    int  listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;

    struct sockaddr_in   clientaddr, serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(def_SERV_PORT);
    
    listenfd = socket(AF_INET, SOCK_STREAM,0);
    if(listenfd == -1)
        printf("socket error \n");
    
    bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));//判定
    listen(listenfd, 4);
    for(;;){
        clilen = sizeof(clientaddr);
        connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clilen);
        if( -1 == connfd )
            perror("accept errror \n");
        childpid = fork();
        if(childpid ==0 ){
            close(listenfd);
            printf("new connect pid=%d\n", getpid());
            str_echo(connfd);
            exit (0);
        }
        printf("goto  father process\n");
        close(connfd);
    }
    return 0;
}

void str_echo(int fd){
    char buf[MAXLINE];
    ssize_t  readn;

again:

    while((readn = read(fd, buf, MAXLINE)) > 0)
        Writen(fd, buf, readn);
    if( readn<0 && errno == EINTR )
        goto again;
    else if(readn<0)
        perror("read error\n");
     printf("read over ===%s \n", buf); 
}
