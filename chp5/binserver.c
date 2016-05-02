#include"sys/socket.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"unistd.h"
#include"netinet/in.h"
#include"../lib/unp.h"
#include"../lib/readn.c"
#include"../tcpcliserv/sum.h"

#include"../lib/error.c"
#include"../lib/writen.c"
#include"../lib/readline.c"

#define def_SERV_PORT 6666
#define MAXLINE 4096

void str_echo(int sockfd)
{
	ssize_t			n;
	struct args		args;
	struct result	result;

	for ( ; ; ) {
		if ( (n = Readn(sockfd, &args, sizeof(args))) == 0)
			return;		/* connection closed by other end */
        printf("..read is:..%d..%d\n", args.arg1, args.arg2);
		result.sum = args.arg1 + args.arg2;
		Writen(sockfd, &result, sizeof(result));
	}
}

void str_echo2(int sockfd);
void str_echo1(int );
int main()
{
    int  listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    int   peerlen;

    struct sockaddr_in   clientaddr, serveraddr, perrsock2;
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
        printf("client addr is.... %s\n", inet_ntoa(clientaddr.sin_addr));
        printf("client prot is.... %d\n", ntohs(clientaddr.sin_port));
        peerlen = sizeof(perrsock2);
        //getpeername(connfd, &perrsock2, (socklen_t*)&peerlen);
        getpeername(connfd, (struct sockaddr*)&perrsock2, NULL);
        printf("server addr is.... %s\n", inet_ntoa(perrsock2.sin_addr));
        printf("server prot is.... %d\n", ntohs(perrsock2.sin_port));
        if( -1 == connfd )
            perror("accept errror \n");
        childpid = fork();
        if(childpid ==0 ){
            close(listenfd);
            printf("new connect pid=%d\n", getpid());
            //str_echo(connfd);
            str_echo(connfd);
            exit (0);
        }
        printf("goto  father process\n");
        close(connfd);
    }
    return 0;
}

void writen2(int fd, char* buf, int len)
{
    int readn;
    int nleft = len;
    while(nleft > 0)
    {
        readn = write( fd, buf, nleft );
        if(readn == 0){
            printf("read over \n");
            return ;
        }
        if(readn < 0 && errno == EINTR)
            readn = 0;
        
            nleft -= readn;
            buf += readn;
            
    }
}

void str_echo1(int fd){
    char buf[MAXLINE];
    ssize_t  readn;

again:

    while((readn = read(fd, buf, MAXLINE)) > 0){
        writen2(fd, buf, readn);
    if( readn<0 && errno == EINTR )
        goto again;
    else if(readn<0)
    {
        perror("read error\n");
        return ;
    }
     printf("read over ===%s \n", buf); 
}
}

void str_echo2(int sockfd)
{
	long		arg1, arg2;
	ssize_t		n;
	char		line[MAXLINE];

	for ( ; ; ) {
		if ( (n = Readline(sockfd, line, MAXLINE)) == 0)
			return;		/* connection closed by other end */

		if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2)
			snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
		else
			snprintf(line, sizeof(line), "input error\n");

		n = strlen(line);
		Writen(sockfd, line, n);
	}
}
