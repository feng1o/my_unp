/*************************************************************************
    > File Name: my-host-serv.c
  > Author: 
  > Mail: 
  > Created Time: Fri 06 May 2016 10:56:33 PM CST
 ************************************************************************/

#include<stdio.h>
#include"netdb.h"

struct addrinfo *host_serv(const char* hostname, const char* service, int family, int socktype){
    struct addrinfo  hints, *result;

    bitzero(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = AI_CANONNAME;

    if(getaddrinfo(hostname, service, &hints, &result) != 0)
        return (NULL);
    return result;
}

