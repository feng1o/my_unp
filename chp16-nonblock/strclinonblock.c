/*************************************************************************
  > File Name: strclinonblock.c
  > Author: 
  > Mail: 
  > Created Time: Tue 24 May 2016 08:33:23 PM CST
 ************************************************************************/

#include<stdio.h>
#include<errno.h>
#include"unistd.h"
#include"sys/socket.h"
#include"fcntl.h"
#include<sys/un.h>

#define MAXLINE   4096
void myStr_cli(FILE *fp, int sockfd){
    int  maxfdp1, val, stdineof;
    ssize_t n, nwritten;
    fd_set  rset, wset; //监听select到描述符号集合
    char to[MAXLINE], fr[MAXLINE];
    char *toiptr, *tooptr, *friptr, *froptr;

    val = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, val|O_NONBLOCK);  //设置为非阻塞格式

    val = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    val = fcntl(STDOUT_FILENO, F_GETFL, 0);
    fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);

    toiptr = tooptr = to; //指针到头
    friptr = froptr = fr;
    stdineof  = 0;

    maxfdp1 = (STDIN_FILENO > STDOUT_FILENO ? STDIN_FILENO : STDOUT_FILENO) >sockfd  ? (STDIN_FILENO > STDOUT_FILENO ? STDIN_FILENO : STDOUT_FILENO):sockfd;

    for(;;){
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if(stdineof == 0 && toiptr < &to[MAXLINE])
            FD_SET(STDIN_FILENO, &rset);
        if(friptr < &fr[MAXLINE])
            FD_SET(sockfd, &rset);

        if(tooptr < toiptr)
            FD_SET(sockfd, &wset);
        if(froptr != friptr)
            FD_SET(STDOUT_FILENO, &wset);

        select(maxfdp1, &rset, &wset, NULL, NULL);
    }

#ifdef	VOL2
				fprintf(stderr, "%s: EOF on stdin\n", gf_time());
#endif
				stdineof = 1;			/* all done with stdin */
				if (tooptr == toiptr)
					shutdown(sockfd, SHUT_WR);/* send FIN */

			} else {
#ifdef	VOL2
				fprintf(stderr, "%s: read %d bytes from stdin\n", gf_time(), n);
#endif
				toiptr += n;			/* # just read */
				FD_SET(sockfd, &wset);	/* try and write to socket below */
			}
		}

		if (FD_ISSET(sockfd, &rset)) {
			if ( (n = read(sockfd, friptr, &fr[MAXLINE] - friptr)) < 0) {
				if (errno != EWOULDBLOCK)
					printf("read error on socket");

			} else if (n == 0) {
#ifdef	VOL2
				fprintf(stderr, "%s: EOF on socket\n", gf_time());
#endif
				if (stdineof)
					return;		/* normal termination */
				else
					err_quit("str_cli: server terminated prematurely");

			} else {
#ifdef	VOL2
				fprintf(stderr, "%s: read %d bytes from socket\n",
								gf_time(), n);
#endif
				friptr += n;		/* # just read */
				FD_SET(STDOUT_FILENO, &wset);	/* try and write below */
			}
		}

}


