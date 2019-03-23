#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<netinet/in.h>


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
        printf("listen!\n");
        return 2;
    }
    //2.绑定信息
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
    //3.改套接字状态为监听
    
    if(listen(listen_sock,5)<0)
    {
      printf("listen!\n");
      return 4;
    }

    //4.循环服务
    while(1)
    {
      int cli_sock = accept(listen_sock,(struct sockaddr*)&addr,&len);
      if(cli_sock<0)
      {
        printf("accept!\n");
        return 0;
      }
      pid_t pid = fork();
      //通过子进程来服务客户端
      if(pid == 0)
      {
        //子进程再创建子进程，此时由孙子进程来处理客户端
        //子进程自杀被父进程收尸，孙子进程成为孤儿进程，由1号进程收尸
        pid_t son = fork();
        if(son > 0)
          _exit(0);
        if(son == 0)
        {
          while(1)
          {
            char buf[1024] = {0};
            int sz = read(cli_sock,buf,1023);
            if(sz == 0)
            {
              printf("client say goodbye!\n");
              break;
            }
            if(sz < 0)
            {
              printf("read!\n");
              break;
            }
            printf("client say:[%s]\n",buf);
            write(cli_sock,buf,strlen(buf));
          }
        }
      }
    wait(NULL);
    }
    close(listen_sock);
    return 0;
}



     

