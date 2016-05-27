#include	"../lib/unp.h"
#include"sys/un.h"

int main(int argc, char **argv)
{
    int sockfd;
    socklen_t len;
    
    struct sockaddr_un  addr1, addr2;

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    //if(sockfd ...)
    unlink(argv[1]);
    memset(&addr1, 0, sizeof(addr1));
    addr1.sun_family = AF_LOCAL;
    strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path-1));
    
    len = sizeof(addr2);
    bind(sockfd, (struct sockaddr*)&addr1, SUN_LEN(&addr1));
    getsockname(sockfd, (struct sockaddr*)&addr2, &len);
    
    printf("bound name =%s,return len =%d\n", addr2.sun_path, len);
	exit(0);
}
