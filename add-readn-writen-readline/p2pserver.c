#include"unistd.h"
#include"stdlib.h"
#include"errno.h"
#include"stdio.h"
#include"netinet/in.h"
#include"arpa/inet.h"
#include"string.h"

#include "signal.h"

#include"sys/types.h" 
#include"sys/socket.h"

void handler(int sig)//信号处理函数  信号发射接收后处理
{
	printf("receive sig\n");
	exit(0); //自身退出
}

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

	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)//这个是结局地址重复利用，否则当服务器关闭后重启，bind到时候会提示地址使用中，网络处于timewait状态，，netstat查看
		//RESUSEADDR就所使得在timewait状态人然可重启服务器
		ERR_EXIT("SET_sockopt");
	//绑定套接子
	if ((bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) < 0)
		ERR_EXIT("bind");

	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);

	int conn;
	if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)//peerlen必须初始化
		ERR_EXIT("accept");
	printf("ip :%s,port = %d", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));


	/*创建进程，这里是一个server队一个client;;为了保证接收发送同时，就2个进程；父进程用来获取数据，子进程监听？*/
	pid_t pid;
	pid = fork();
	if (-1 == pid)
	{
		ERR_EXIT("FORK");
	}
	if (0==pid)
	{	
		signal(SIGUSR1, handler);//SIGUSR是linux下kill l查看； 用信号，handler自定的一个信号，父进程退出的时候向子进程发射信号
		char sendbuf[1024] = { 0 }; //发送数据进程
		while (fgets(sendbuf,sizeof(sendbuf),stdin)!=NULL)
		{
			write(conn, sendbuf, strlen(sendbuf)); //通过conn发送出去
			memset(sendbuf, 0, sizeof(sendbuf));//发送完就清空，防止下次发送有前面的部分，比如前面长点
		}
		printf("child closed");
		exit(EXIT_SUCCESS);
	}
	else
	{
		char recvbuf[1024];
		while (1)
		{
			memset(recvbuf, 0, sizeof(recvbuf));
			int ret = read(conn, recvbuf, sizeof(recvbuf));
			if (ret == -1)
			{
				ERR_EXIT("read");
			}
			else if (0==ret)
			{
				printf("perr_close\n");
				break;//不跳出循环，在客户端关了会导致一直在这里面循环  进程执行完毕
			}
			fputs(recvbuf,stdout);
			//printf("%s", recvbuf);
			//write(conn, recvbuf, ret);

		}
		printf("parent process closed");//此程序在client关闭后会发现父进程关闭了，但是子进程并未关闭，，父进程退出时候可通过信号来通知子进程
		kill(pid, SIGUSR1);//进程pid，信号名字；；父进程退出的时候发射这个信号给子进程
		exit(EXIT_SUCCESS);
	}
	

	return 0 ;
}

