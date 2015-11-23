
#include"unistd.h"
#include"stdlib.h"
#include"errno.h"
#include"stdio.h"
#include"string.h"

/*返回读取的字节数，告知读取了多少，剩余多少 返回-1出错*/
ssize_t  readn(int fd, char *buf, size_t nbytes)
{
	size_t nread;
	size_t nleft;//剩余要读取的

	char * bufp = (char *)buf;
	nleft = nbytes;  //初始化

	while (nleft>0)//
	{
		if (nread=read(fd,bufp,nleft)<0)//read从fd文件描述符文件读取nread 字节，请求读nleft但是未必能读nlfet；存入bufp
		{
			if (errno = EINTR)//读取的字节小0，有两种情况，第一种是中断引起的，并没出错，继续读
			{
				nread = 0;
				continue;
			}
			else
			{
				return -1;//出错了这个、失败返回-1
			}
		}
		else if (0 == nread) //督导结尾
		{
			break;
		}
		nleft -= nread;
		bufp += nread;//buffer偏移

	}
	return (nbytes - nread);
}

/*返回xiru的字节数，告知读取了多少，剩余多少 返回-1出错*/
ssize_t writen(int fd, char *buf, size_t nbytes)
{
	size_t nwrite;
	size_t nleft;//剩余要读取的

	char * bufp = (char *)buf;
	nleft = nbytes;  //初始化

	while (nleft>0)//
	{
		if (nwrite=write(fd,bufp,nleft)<0)//read从fd文件描述符文件读取nread 字节，请求读nleft但是未必能读nlfet；存入bufp
		{
			if (errno = EINTR)//读取的字节小0，有两种情况，第一种是中断引起的，并没出错，继续读
			{
				nwrite = 0;
				continue;
			}
			else
			{
				return -1;//出错了这个、失败返回-1
			}
		}
		else if (0 == nwrite) //督导结尾
		{
			continue;
		}
		nleft -= nwrite;
		bufp += nwrite;//buffer偏移

	}
	return (nbytes);
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
  while(1)
    {
       int ret = ssize_t recv(sockfd, buf, len, int MSG_PEEK);
       if(ret == -1 && errno==EINTR)
            continue;
       return  ret;

    }
}


ssize_t readline(int sockefd,void *buf,size_t maxline)
{
    int ret ;
    int nread;
    char *bufp = buf;
    int nleft = maxline;

    while(1)
    {
        ret = recv_peek(sockfd,bufp,nleft);
        if(ret<0)
            return ret;
        else if(ret==0)
            return ret;
        nread = ret;
        int i;
        for(i=0;i<nread;i++)
        {
            if(bufp[i]=='\n')
            {
                ret = readn(sockfd,bufp,i+1);
                if(ret!=i+1)
                    exit(EXIT_FAILURE);

                return ret;
            }
        }
        
        if(nread > nleft)
            exit(EXIT_FAILURE);
        nleft -= nread;
        ret = readn(sockfd,bufp,nread);
		if(ret!=nread)
			exit(EXIT_FAILURE);
		bufp += nread;

    }
	return -1;
}
