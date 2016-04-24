/*************************************************************************
  > File Name: inetpton.c
  > Author: 
  > Mail: 
  > Created Time: Sun 24 Apr 2016 07:06:38 PM CST
 ************************************************************************/

#include<stdio.h>
#include"netinet/in.h"
#include"arpa/inet.h"

int main()
{
    struct in_addr addr;
    char *str;
    if(inet_aton("127.0.0.1", &addr))
    printf("ok\n");

    str = inet_ntoa(addr);
    
    printf("127.0.0.1 is = %s\n",str);    
    return 0;
}
