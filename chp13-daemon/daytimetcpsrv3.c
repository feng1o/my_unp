#include	"../lib/unp.h"
#include"../lib/wraplib.c"
#include"../lib/wrapunix.c"
#include"../lib/wrapsock.c"
#include"../lib/error.c"
#include"../lib/daemon_init.c"
#include"../lib/tcp_listen.c"

#include"../lib/daemon_inetd.c"
#include"../lib/wrapstdio.c"
#include"../lib/signal.c"
#include"../lib/sock_ntop.c"
#include	<time.h>

int main(int argc, char **argv)
{
	socklen_t		len;
	struct sockaddr	*cliaddr;
	char			buff[MAXLINE];
	time_t			ticks;

	daemon_inetd(argv[0], 0);

	cliaddr = Malloc(sizeof(struct sockaddr_storage));
	len = sizeof(struct sockaddr_storage);
	Getpeername(0, cliaddr, &len);
	err_msg("connection from %s", Sock_ntop(cliaddr, len));

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
    Write(0, buff, strlen(buff));

	Close(0);	/* close TCP connection */
	exit(0);
}
