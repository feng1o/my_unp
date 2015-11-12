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

#define ERR_EXIT(m) \ 
do \
{ \
perror(m); \
exit(EXIT_FAILURE); \
}  \
while (0)

void handler(int sig)//信号处理函数  信号发射接收后处理
{
	printf("receive sig %d\n",sig);
	exit(0); //自身退出
}

int main(void)
{
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	//listenfd = socket(PF_INET,SOCKET_STREAM,IPPROTO_TCP);

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockadd *)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("connect");//链接失败
	
	/*子进程接收父进程发送数据*/
	pid_t pid;
	pid = fork();
	if (-1==pid)
	{
		ERR_EXIT("fork");
	}
	if (pid == 0)
	{
		char recvbuf[1024];
		while (1) //子进程接收
		{
			memset(recvbuf,0,sizeof(recvbuf));
			int ret = read(sock, recvbuf, sizeof(recvbuf));
			if (ret == -1)
			{
				ERR_EXIT("read");
			}
			else if (0==ret) //就是对等方关闭，双发都可以主动关闭
			{
				printf("perr closed");
				break;//不跳出循环，在客户端关了会导致一直在这里面循环  进程执行完毕
			}
			fputs(recvbuf, stdout); //否则大于0就是输出来
		}
		close(sock);
		kill(getppid(), SIGUSR1);//getppid获取父进程的id；；
		printf("child process shutdown\n");
	}
	else //父进程发送
	{

		signal(SIGUSR1, handler);
		char sendbuf[1024] = { 0 };
		//char recvbuf[1024] = { 0 };
		while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL){
			write(sock, sendbuf, strlen(sendbuf));
			//read(sock, recvbuf, sizeof(recvbuf));
			//fputs(recvbuf, stdout);
			//memset(recvbuf, 0, sizeof(recvbuf));//这两句主要所清空缓冲区，否则第一次输入长的，后面短到，会发生问题，第二次后面部分保留第一次输入到数据
			memset(sendbuf, 0, sizeof(sendbuf));
		}
		close(sock);

	}
	return 0;

}
/*对于client在链接到时候就确定socekt地址*/

