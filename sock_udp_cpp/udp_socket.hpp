#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
using namespace std;
#include<string.h>


class Udp_socket
{
  public:
    Udp_socket()
      :_fd(-1)
    {}
    Udp_socket(int fd)
      :_fd(fd)
    {}
    //关闭套接字
    bool Close()
    {
      close(_fd);
      return true;
    }
    //创建套接字
    bool Socket()
    {
      if((_fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))<0)
      {
        cout<<"socket!\n";
        return false;
      }
      return true;
    }
    //绑定
    bool Bind(const string& ip,const uint16_t& port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);
      socklen_t len = sizeof(addr);
      if((bind(_fd,(struct sockaddr*)&addr,len))<0)
      {
        cout<<"bind!\n";
        return false;
      }
      return true;
    }
    //接收消息
    bool RecvFrom(string* str,string* ip = NULL,uint16_t* port = NULL)
    {
      printf("enter recv!\n");
      char buf[1024*10] = {0};
      struct sockaddr_in addr;
      socklen_t len = sizeof(addr);
      ssize_t rz = recvfrom(_fd,buf,sizeof(buf)-1,0,(struct sockaddr*)&addr,&len);
      if(rz < 0)
      {
        printf("recvfrom!\n");
        return false;
      }
      //读取成功，将参数放入缓冲区和ip port中
      *str = buf;
      if(ip != NULL)
      {
        *ip = inet_ntoa(addr.sin_addr);
      }
      if(port != NULL)
      {
        *port = addr.sin_port;
      }
      return true;
    }
    //发送消息
    bool SendTo(const std::string& str,const std::string& ip,uint16_t port)
    {
      printf("sock send!\n");
      struct sockaddr_in addr;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);
      addr.sin_family = AF_INET;
      socklen_t len = sizeof(addr);
      
      size_t sz = sendto(_fd,str.data(),str.size(),0,(struct sockaddr*)&addr,len);
      if(sz < 0)
      {
        printf("sendto!\n");
        return false;
      }
      return true;
    }
    //返回_fd
    int GetSock()
    {
      return _fd;
    }
  private:
    int _fd;
};



