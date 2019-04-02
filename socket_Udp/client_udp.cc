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
    printf("./server_udp [IP] [PORT]\n");
    return 1;
  }
  //1.创建套接字
  int udp_sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if(udp_sock<0)
  {
    printf("socket!\n");
    return 2;
  }
  //2.初始化服务端地址信息
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  socklen_t len = sizeof(addr);
  //3.循环和服务端交互
  while(1)
  {
    char buf[1024] = {0};
    printf("client[enter]:");
    fflush(stdout);
    scanf("%s",buf);
    sendto(udp_sock,buf,1024,0,(const struct sockaddr*)&addr,len);
    
    int sz = recvfrom(udp_sock,buf,1024,0,(struct sockaddr*)&addr,&len); 
    if(sz<0)
    {
      printf("read!\n");
    }
    if(sz == 0)
    {
      printf("client say goodbye!\n");
      return 3;
    }
    printf("server responds[%s]\n",buf);
  }
  close(udp_sock);
  return 0;
}
