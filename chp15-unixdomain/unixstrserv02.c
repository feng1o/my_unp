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
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_un	cliaddr, servaddr;
	void				sig_chld(int);

	listenfd = Socket(AF_LOCAL, SOCK_STREAM, 0);

	unlink(UNIXSTR_PATH);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, sig_chld);

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				err_sys("accept error");
		}

		if ( (childpid = Fork()) == 0) {	/* child process */
			Close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}
ssize_t	read_cred(int, void *, size_t, struct cmsgcred *);
void str_echo(int sockfd)
{
	ssize_t			n;
	int			i;
	char			buf[MAXLINE];
	struct cmsgcred	cred;

again:
	while ( (n = read_cred(sockfd, buf, MAXLINE, &cred)) > 0) {
		if (cred.cmcred_ngroups == 0) {
			printf("(no credentials returned)\n");
		} else {
			printf("PID of sender = %d\n", cred.cmcred_pid);
			printf("real user ID = %d\n", cred.cmcred_uid);
			printf("real group ID = %d\n", cred.cmcred_gid);
			printf("effective user ID = %d\n", cred.cmcred_euid);
			printf("%d groups:", cred.cmcred_ngroups - 1);
			for (i = 1; i < cred.cmcred_ngroups; i++)
				printf(" %d", cred.cmcred_groups[i]);
			printf("\n");
		}
		Writen(sockfd, buf, n);
	}

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}

#define	CONTROL_LEN	(sizeof(struct cmsghdr) + sizeof(struct cmsgcred))
ssize_t read_cred(int fd, void *ptr, size_t nbytes, struct cmsgcred *cmsgcredptr)
{
	struct msghdr	msg;
	struct iovec	iov[1];
	char			control[CONTROL_LEN];
	int				n;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	msg.msg_controllen = sizeof(control);
	msg.msg_flags = 0;
	if ( (n = recvmsg(fd, &msg, 0)) < 0)
		return(n);

	cmsgcredptr->cmcred_ngroups = 0;	/* indicates no credentials returned */
	if (cmsgcredptr && msg.msg_controllen > 0) {
		struct cmsghdr	*cmptr = (struct cmsghdr *) control;

		if (cmptr->cmsg_len < CONTROL_LEN)
			err_quit("control length = %d", cmptr->cmsg_len);
		if (cmptr->cmsg_level != SOL_SOCKET)
			err_quit("control level != SOL_SOCKET");
		if (cmptr->cmsg_type != SCM_CREDS)
			err_quit("control type != SCM_CREDS");
		memcpy(cmsgcredptr, CMSG_DATA(cmptr), sizeof(struct cmsgcred));
	}
	return(n);
}
