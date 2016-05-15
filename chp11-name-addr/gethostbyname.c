/*************************************************************************
    > File Name: gethostbyname.c
  > Author: 
  > Mail: 
  > Created Time: Thu 05 May 2016 08:22:30 PM CST
 ************************************************************************/

#include<stdio.h>
#include"stdlib.h"
#include"sys/socket.h"
#include"netinet/in.h"
#include"arpa/inet.h"
#include"netdb.h"


int main(int argc, char** argv)
{
    if(argc < 2)
        perror("input error,,hostname please\n");
    struct hostent  *hptr;
    char *pname, **ppalias;
    char str[INET_ADDRSTRLEN];

    while( --argc > 0 )
    {
        pname = *++argv;
        if( (hptr = gethostbyname(pname)) == NULL ){
            printf("gethostbyname error: host :%s, :%s", pname, hstrerror(h_errno));   
            continue;
        }
        printf("official name: %s\n", hptr->h_name);
        for( ppalias = hptr->h_aliases; *ppalias != NULL; ++ppalias ){
            printf("\talias name : %s\n", *ppalias);
        }
        
        switch(hptr->h_addrtype)
        {
            case AF_INET:
                ppalias = hptr->h_addr_list;
                for(; ppalias != NULL; ++ppalias)
                    printf("\taddress : %s \n", inet_ntop(hptr->h_addrtype, *ppalias, str, sizeof(str)));
                break;
            default:
                perror("unknow address \n");
                break;
        }
    }
    return 0;
}

