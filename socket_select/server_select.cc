#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>


//重置readfds，将数组中的描述符加到readfds位图中
void Reload(int listen_sock,int* num,int size,int* max,fd_set* readfds)
{
  FD_ZERO(readfds);
  FD_SET(listen_sock,readfds);
  int temp_max = listen_sock;
  for(int i = 0;i<size;i++)
  {
    if(num[i] != -1)
      FD_SET(num[i],readfds);
    if(num[i]>temp_max)
      temp_max = num[i];
  }
  *max = temp_max;
}

//将connect_fd加入到num数组中
void Add(int connect_fd,int* num,int size)
{
  for(int i = 0;i<size;i++)
  {
    if(num[i] == -1)
    {
      num[i] = connect_fd;
      break;
    }
  }
  return;
}


int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./server [IP] [PORT]\n");
    return 1;
  }
  //1.创建套接字
  int listen_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(listen_sock<0)
  {
    printf("socket!\n");
    return 2;
  }
  //2.为套接字绑定地址信息
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  addr.sin_port = htons(atoi(argv[2]));
  socklen_t len = sizeof(addr);

  if(bind(listen_sock,(struct sockaddr*)&addr,len)<0)
  {
    printf("bind!\n");
    return 3;
  }
  //3.改套接字状态为监听状态
  if(listen(listen_sock,5)<0)
  {
    printf("listen!\n");
    return 4;
  }
  //多路转接的处理
  int num[1024] = {-1};
  int max = listen_sock;
  fd_set readfds;
  FD_SET(listen_sock,&readfds);
  for(int i = 0;i<1024;i++)
    num[i] = -1;
  //4.循环接收客户端请求并服务
  while(1)
  {
    //因为select每次都会更改readfds，所以每次上来先重置readfds
    max = listen_sock;
    Reload(listen_sock,num,sizeof(num)/sizeof(num[0]),&max,&readfds);
    printf("before select :%d\n",FD_ISSET(listen_sock,&readfds));
    int ret = select(max+1,&readfds,NULL,NULL,NULL);
    printf("after select :%d\n",FD_ISSET(listen_sock,&readfds));
    if(ret<0)
    {
      perror("select!\n");
      continue;
    }
    if(ret == 0)
    {
      printf("time out!\n");
      continue;
    }
    //处理listen_sock
    if(FD_ISSET(listen_sock,&readfds))
    {
      //处理客户端地址
      struct sockaddr_in cli_addr;
      socklen_t len = sizeof(cli_addr);
      int connect_fd = accept(listen_sock,(struct sockaddr*)&cli_addr,&len);
      if(connect_fd<0)
      {
        printf("accept!\n");
        continue;
      }
      printf("client %s:%d\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
      Add(connect_fd,num,sizeof(num)/sizeof(num[0]));
    }
    //处理connect_fd
    for(int i = 0;i<sizeof(num)/sizeof(num[0]);i++)
    {
      if(num[i] == -1)
        continue;
      if(!FD_ISSET(num[i],&readfds))
        continue;
      //对可读文件描述符进行处理
      char buf[1024] = {0};
      ssize_t readsz = read(num[i],&buf,1023);
      if(readsz == 0)
      {
        printf("client say goodbye!\n");
        num[i] = -1;
        close(num[i]);
      }
      printf("client say :[%s]\n",buf);
      write(num[i],buf,strlen(buf));
    }
  }
  return 0;
}
