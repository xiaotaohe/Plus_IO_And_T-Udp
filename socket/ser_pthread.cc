#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

int ser_sock = 0;
void* run(void* arg)
{
  int cli_sock = *((int*)arg);
  while(1)
  {
    char buf[1024] = {0};
    memset(buf,0,1024*sizeof(char));
    int rz = read(cli_sock,buf,1023);
    if(rz<0)
    {
      printf("read!\n");
      return (void*)1;
    }
    if(rz == 0)
    {
      printf("client say[goodbye]\n");
      return 0;
    }
    printf("client say:[%s]\n",buf);
    write(cli_sock,buf,strlen(buf));
  }
}
int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./server0 [IP] [PORT]\n");
    return 1;
  }
  //创建套接字
  ser_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(ser_sock<0)
  {
    printf("socket!\n");
    return 2;
  }
  //绑定地址信息
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  socklen_t len = sizeof(addr);

  int opt = 1;
  setsockopt(ser_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));//可以让服务器立即重启，
  if(bind(ser_sock,(const struct sockaddr*)&addr,len)<0)
  {
    printf("bind\n");
    return 3;
  }
  //将套接字的状态改为监听状态
  if(listen(ser_sock,5)<0)
  {
    printf("listen!\n");
    return 4;
  }

  //利用多线程处理
  pthread_t tid;
  struct sockaddr_in cli_addr;
  
  while(1)
  {
      int cli_sock = accept(ser_sock,(struct sockaddr*)&cli_addr,&len);
      if(cli_sock<0)
      {
        printf("accept!\n");
        continue;
      }
      printf("get a new link!!! cli_sock = %d\n",cli_sock);
      pthread_create(&tid,NULL,run,(void*)&cli_sock);
      pthread_detach(tid);
  }
  return 0;
}
