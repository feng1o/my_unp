#include"unistd.h"
#include"sys/socket.h"
#include"netinet/in.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"arpa/inet.h"

#define MAXLINE 4096

void str_client(int);

int main()
{
    int sockfd;
    struct sockaddr_in  serveraddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serveraddr, 0, sizeof(serveraddr));

    inet_aton("127.0.0.1", &(serveraddr.sin_addr)); 
    serveraddr.sin_port = htons(6666); 
    serveraddr.sin_family = AF_INET;

    connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    str_client(sockfd);
    return 0;
}

void str_client(int fd)
{
    char buf[MAXLINE];
    write(fd, "xxxxxxxxxxx", 1000);
    read(fd, buf, MAXLINE);
    printf("....%s\n", buf);

}
