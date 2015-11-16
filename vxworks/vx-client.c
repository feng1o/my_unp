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

/* 
define different types label
*/ 
#define  SAR_STATE          0
#define  RF_PARAMETER_SET   1
#define  READ_FILELIST      2
#define  READ_FILE          3
#define  DELETE_FILE        4
#define  FORMAT_FILE        5
#define  RF_PARAMETER_TEST  6

/* 
pack  send messages
*/ 

typedef struct messge_pack
{
	char cmd_type;
	char str[20];
	
}messge_pack;

int main(void ){
	int sock;
	if((sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	ERR_EXIT("socket");
	//listenfd = socket(PF_INET,SOCKET_STREAM,IPPROTO_TCP);
	
	struct sockaddr_in servaddr;	
	memset(&servaddr ,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sock,(struct sockadd *)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("connect");//链接失败
	
	//char sendbuf[1024] = {0};
	//char recvbuf[1024] = {0};

	//while(fgets(sendbuf	,sizeof(sendbuf),stdin)!=NULL){
		   // printf("gets = %s",sendbuf);
	struct messge_pack sendbuf={READ_FILELIST,"read flie list"};
	//sendbuf.cmd_type =  READ_FILELIST;
	//sendbuf.str = "read filelist";// ERROR  cannot asignmet a data array like this; just can initailize when define a variable
	write(sock,&sendbuf	,sizeof(sendbuf));
            printf("send is= %s\n",&sendbuf.str);
	printf("type cmd is= %d\n",sendbuf.cmd_type);
       // read(sock,recvbuf ,sizeof(recvbuf));
		//fputs(recvbuf,stdout);
		//memset(recvbuf,0,sizeof(recvbuf));//这两句主要所清空缓冲区，否则第一次输入长的，后面短到，会发生问题，第二次后面部分保留第一次输入到数据
		//memset(sendbuf,0,sizeof(sendbuf));
//	}
	close(sock);
	
	}
/*对于client在链接到时候就确定socekt地址*/

