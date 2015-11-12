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

struct packet
{
    int len;
    char buf[1024];
};

int main(){
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
	
	if(connect(sock, (struct sockadd *)&servaddr,sizeof(servaddr))<0)
		ERR_EXIT("connect");//链接失败
    else 
        printf("connected\n");
    struct  packet sendbuf; //dingyi 
    struct  packet  recvbuf;
    memset(&sendbuf,0,sizeof(sendbuf));//clear memory
    memset(&recvbuf,0,sizeof(recvbuf));
	int n;
    char  *str;
    while(fgets(sendbuf.buf,sizeof(sendbuf.buf),stdin)!=NULL)
    {
  //  while(fgets(str,strlen(str),stdin)!=NULL){
		printf("input over,start send \n");
        printf("input is:%s\n",sendbuf.buf);
        //sendbuf.buf = str;
        n = strlen(sendbuf.buf);
        printf("send %d bytes\n", n);
        sendbuf.len = htonl(n); //intenet byte 
        writen(sock,&sendbuf,4+n);
	
#if 0 
		int ret = readn(sock, &recvbuf.len, 4);
        if (-1==ret)
            ERR_EXIT("READ");
		if (ret < 4)//就是client关闭了
		{
			printf("client_closed\n");
			break;//chun来
		}

        n = ntohl(recvbuf.len);
       ret =  readn(sock, recvbuf.buf, n);//接受包提
        if (-1==ret)
            ERR_EXIT("READ");
		if (ret < n)//就是client关闭了
		{
			printf("client_closed\n");
			break;//chun来
		
        }
#endif
        fputs(recvbuf.buf ,stdout);
        // printf("%s", recvbuf);
		//memset(recvbuf,0,sizeof(recvbuf));//这两句主要所清空缓冲区，否则第一次输入长的，后面短到，会发生问题，第二次后面部分保留第一次输入到数据
	
    memset(&sendbuf,0,sizeof(sendbuf));//clear memory
    memset(&recvbuf,0,sizeof(recvbuf));
    }
    //close(sock);
	
	}
/*对于client在链接到时候就确定socekt地址*/
