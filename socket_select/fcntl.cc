/*
 *将文件描述符设置为非阻塞
 *1.轮询的方式读取标准输入
 */
#include<stdio.h>
#include<unistd.h>
#include<sys/fcntl.h>

void SetNonBlock(int fd)
{
  int fl = fcntl(fd,F_GETFD);
  if(fl<0)
  {
    printf("fcntl!\n");
    return;
  }
  fcntl(fd,F_SETFD,fl | O_NONBLOCK);
}
int main()
{
  SetNonBlock(STDIN_FILENO);
  while(1)
  {
    char buf[1024] = {0};
    ssize_t readsz = read(STDIN_FILENO,buf,1024);
    if(readsz<0)
    {
      printf("read!\n");
      sleep(1);
      continue;
    }
    printf("stdin : %s",buf);
  }
  return 0;
}
