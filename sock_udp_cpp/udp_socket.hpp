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
#include<string>


class Udp_socket
{
  public:
    Udp_socket()
      :_fd(-1)
    {}
    Udp_socket(int fd)
      :_fd(fd)
    {}
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
  private:
    int _fd;
};



