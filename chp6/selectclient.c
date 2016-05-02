#include"unistd.h"
#include"../wrapfun.h"
#include"sys/socket.h"
#include"../lib/writen.c"
#include"../lib/readline.c"
#include"netinet/in.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"arpa/inet.h"
#include"errno.h" 
#include"sys/select.h"
#define MAXLINE 4096

void str_get(FILE* fp, int sockfd);
void str_client(int);

void str_cli(FILE *fp, int sockfd);

int main()
{
    int sockfd;
    struct sockaddr_in  serveraddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serveraddr, 0, sizeof(serveraddr));

    inet_aton("127.0.0.3", &(serveraddr.sin_addr)); 
    serveraddr.sin_port = htons(6666); 
    serveraddr.sin_family = AF_INET;

    connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    str_cli(stdin, sockfd);
//    str_get(stdin, sockfd);
//    str_client(sockfd);
    return 0;
}

void str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
            printf("fd_isset sockfd =%d ,number is = %d \n",sockfd,FD_ISSET(sockfd, &rset));
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
void str_get(FILE* fp, int sockfd){
    char sendline[MAXLINE], recvline[MAXLINE];
    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
        write(sockfd, sendline, MAXLINE);    
        //fflush(stdin);
        puts("send.......");
        read(sockfd, recvline, MAXLINE);
        fputs(recvline, stdout);
    }
}

void str_client(int fd)
{
    char buf[MAXLINE];
    write(fd, "xxxxxxxxxxx", 1000);
    read(fd, buf, MAXLINE);
    printf("....%s\n", buf);

}
int  writen2(int fd, char* buf, int n){
    int nleft = n;
    int nwrite ;
    char* buf2 = buf;
    while(nleft > 0)
    {   nwrite = write(fd, buf2, nleft);
        if( 0 > nwrite && errno == EINTR )
            continue;
        else
            return -1;
        nleft -= nwrite;
        buf2 += nwrite;
    }
    return  n;
}

void str_cli222(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		writen2(sockfd, sendline, strlen(sendline));
		//writen2(sockfd, sendline, 1);
		//sleep(1);
		//write(sockfd, sendline+1, strlen(sendline)-1);

		if (read(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");

		fputs(recvline, stdout);
	}
}

