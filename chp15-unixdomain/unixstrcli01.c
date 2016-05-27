#include	"../lib/unp.h"
#include"../lib/wrapsock.c"
#include"../lib/wrapunix.c"
#include"../lib/wrapstdio.c"
#include"../lib/signal.c"
#include"../lib/str_echo.c"
#include"../lib/str_cli.c"

#include"../lib/writen.c"
#include"../lib/wraplib.c"
#include"../lib/error.c"
#include"../lib/readline.c"


int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_un	servaddr;

	sockfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
