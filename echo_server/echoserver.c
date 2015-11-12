#include"unistd.h"
#include"stdlib.h"
#include"errno.h"
#include"stdio.h"
#include"netinet/in.h"
#include"arpa/inet.h"
#include"string.h"

#include"sys/types.h"
#include"sys/socket.h"

#define ERR_EXIT(m) \ 
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	}  \
	while(0)

int main(void ){
	int listenfd;
	if((listenfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	//listenfd = socket(PF_INET,SOCKET_STREAM,IPPROTO_TCP);
	
	struct sockaddr_in servaddr;	
	memset(&servaddr ,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int on = 1;
	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0)//这个是结局地址重复利用，否则当服务器关闭后重启，bind到时候会提示地址使用中，网络处于timewait状态，，netstat查看
		//RESUSEADDR就所使得在timewait状态人然可重启服务器
		ERR_EXIT("SET_sockopt");
	//绑定套接子
	if((bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))) < 0)
	ERR_EXIT("bind");

	if(listen(listenfd,SOMAXCONN) < 0)
		ERR_EXIT("listen");
	
	struct sockaddr_in peeraddr;
	socklen_t peerlen =sizeof(peeraddr);
	
	int conn;
	if((conn = accept(listenfd,(struct sockaddr*)&peeraddr,&peerlen))<0)//peerlen必须初始化
		ERR_EXIT("accept");
	printf("ip :%s,port = %d",inet_ntoa(peeraddr.sin_addr),ntohs(peeraddr.sin_port));
	char recvbuf[1024];
	
	while(1)
	{
  		memset(recvbuf,0,sizeof(recvbuf));
		int ret = read(conn,recvbuf,sizeof(recvbuf));
		fputs(recvbuf,stdout);
		printf("%s",recvbuf);
		write(conn,recvbuf,ret);

 	}

	//return 0 ;
}
