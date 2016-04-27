/*************************************************************************
  > File Name: readn.c
  > Author: 
  > Mail: 
  > Created Time: Sun 24 Apr 2016 07:57:12 PM CST
 ************************************************************************/

#include<stdio.h>
#include "fcntl.h"
#include "unistd.h"
#include "errno.h"

ssize_t readn(int fd, void *vptr, size_t n)
{
    int  nleft = n;
    int  nread;
    char*  ptr = (char*) vptr;

    while(nleft > 0){
    if((nread = read(fd, ptr, nleft))<0)
        if(errno == EINTR)
            nread = 0;
    else
        return  -1;
    else if(0 == nread)
        break;
    
    nleft -= nread;
    ptr = ptr + nread;
    }
    return (n - nleft);
}

