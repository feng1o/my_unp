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

