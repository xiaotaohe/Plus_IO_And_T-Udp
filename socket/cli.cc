#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./client [IP] [port]\n");
    return 1;
  }
  //创建套接字
  int cli_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(cli_sock<0)
  {
    printf("socket!\n");
    return 2;
  }
  //连接服务端
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  socklen_t len = sizeof(addr);
  int ret = connect(cli_sock,(const struct sockaddr*)&addr,len);
  if(ret<0)
  {
    printf("connect!\n");
    return 0;
  }
  while(1)
  {
    char buf[1024]={0};
    printf("client[enter]:");
    fflush(stdout);
    scanf("%s",buf);
    write(cli_sock,buf,1024);
    read(cli_sock,buf,1023);
    printf("server[say]:%s\n",buf);
  }
  close(cli_sock);
}
