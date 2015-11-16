#include"unistd.h"
#include"stdlib.h"
#include"errno.h"
#include"stdio.h"
#include"netinet/in.h"
#include"arpa/inet.h"
#include"string.h"

#include"sys/types.h"
#include"sys/socket.h"

void do_service(int conn)
{

	char recvbuf[1024];
	while (1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		int ret = read(conn, recvbuf, sizeof(recvbuf));
		if (0==ret)//就是client关闭了
		{
			printf("client_closed\n");
			break;//chun来
		}
		//fputs("%s",recvbuf);
		printf("%s", recvbuf);
		write(conn, recvbuf, ret);

	}
}
//���� byliu
#define ERR_EXIT(m) \ 
do \
{ \
perror(m); \
exit(EXIT_FAILURE); \
}  \
while (0)

int main(void){
	int listenfd;
	if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	//listenfd = socket(PF_INET,SOCKET_STREAM,IPPROTO_TCP);

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/*解决地址复用，服务器重启启动bind 地址在使用中的问题*/
	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)//这个是结局地址重复利用，否则当服务器关闭后重启，bind到时候会提示地址使用中，网络处于timewait状态，，netstat查看
		//RESUSEADDR就所使得在timewait状态人然可重启服务器
		//solve addres resuse; when close server,can not  restart it ,with error address is in using;
		ERR_EXIT("SET_sockopt");
	//绑定套接子
	if ((bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0)
		ERR_EXIT("bind");

	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);

	int conn;

	/*解决多个客户端连接的问题，创建进程；；思路：获得连接就创建一个子进程做连接的事情，父进程出去监听*/
	pid_t pid;
	while (1)
	{
		if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)//peerlen必须初始化
			ERR_EXIT("accept");
		printf("ip :%s,port = %d", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

		/* */
		pid = fork();//SO_REUSEADDR make it possible for  multi client connect to zhe server 
		if (-1 == pid)
			ERR_EXIT("fork");
		if (0 == pid) //pid 0就是表示自己？？？  pid 0 is its self,sub thred
		{
			close(listenfd);//listen因为子进程不需要监听/close listen,as sub thred need not listen;
			do_service(conn);//z这里面他会发给对应的套接字  进程会在这里面循环
			exit(EXIT_SUCCESS);//客户端关闭了，这个进程就销毁掉
		}
		else
			close(conn);

	}
	return 0;
}


