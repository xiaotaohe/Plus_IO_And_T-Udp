#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./client [ip] [port]\n");
    return 1;
  }
  //1.创建端口号
  int fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if(fd < 0)
  {
    printf("socket!\n");
    return 2;
  }
  
  //循环发送接收消息
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  socklen_t len = sizeof(addr);

  for(;;)
  {
    char buf[1024]={0};
    printf("enter#");
    fflush(stdout);
    scanf("%s",buf);
    ssize_t sz = sendto(fd,buf,sizeof(buf)-1,0,(struct sockaddr*)&addr,len);
    if(sz<0)
    {
      printf("send!\n");
      return 3;
    }
    ssize_t rz = recvfrom(fd,buf,sizeof(buf)-1,0,(struct sockaddr*)&addr,&len);
    if(rz<0)
    {
      printf("recv!\n");
      return 4;
    }
    printf("%s\n",buf);
  }
}
