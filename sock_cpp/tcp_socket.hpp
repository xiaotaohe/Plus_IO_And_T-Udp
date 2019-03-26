/*
 *将tcp封装起来，通过C++
 */
#pragma once

#include<iostream>
#include<string>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<arpa/inet.h>
#include<netinet/in.h>


#define CHECK_RET(exp) if(!exp){\
return false;\
}

class Tcp_socket
{
  public:
    //tcp_socket构造函数
    Tcp_socket()
      :_fd(-1)
    {}
    Tcp_socket(int fd)
      :_fd(fd)
    {}
    //创建套接字Socket
    bool Socket()
    {
      if((_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
      {
        std::cout<<"socket!\n";
        return false;
      }
      return true;
    }
    //关闭套接字的接口
    bool Close()
    {
      close(_fd);
      std::cout<<"close fd = "<<_fd<<std::endl;
      return true;
    }
    //绑定地址信息接口
    bool Bind(const std::string& ip,const uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);
      socklen_t len = sizeof(addr);

      if((bind(_fd,(struct sockaddr*)&addr,len))<0)
      {
        std::cout<<"bind!\n";
        return false;
      }
      return true;
    }
    //改套接字的状态为监听套接字
    bool Listen()
    {
      if(listen(_fd,5)<0)
      {
        std::cout<<"listen\n";
        return false;
      }
      return true;
    }
    //接收connect请求的套接字
    bool Accept(Tcp_socket* peer,std::string* ip = NULL,uint16_t* port = NULL)
    {
      //初始化client套接字
      struct sockaddr_in peer_addr;
      socklen_t len = sizeof(peer_addr);
      
      //接收connect请求
      int ret = accept(_fd,(struct sockaddr*)&peer_addr,&len);
      if(ret<0)
      {
        std::cout<<"accept!\n";
        return false;
      }
      std::cout<<"accept new fd = "<<ret<<std::endl;
      peer->_fd = ret;
      if(NULL != ip)
        *ip = inet_ntoa(peer_addr.sin_addr);
      if(NULL != port)
        *port = ntohs(peer_addr.sin_port);
      return true;
    }
    //读取数据
    bool Recv(std::string* buf)
    {
      buf->clear();
      char temp_buf[1024*10] = {0};
      int readsz = recv(_fd,temp_buf,sizeof(temp_buf),0);
      if(readsz<0)
      {
        printf("read!\n");
        return false;
      }
      if(readsz == 0)
      {
        return false;
      }
      buf->assign(temp_buf,readsz);
      return true;
    }

    //发送
    bool Send(const std::string& str) const
    {
      ssize_t write_sz = send(_fd,str.data(),str.size(),0);
      if(write_sz<0)
      {
        printf("send!\n");
        return false;
      }
      else
          return true;
    }
    //连接
    bool Connect(const std::string& ip,const uint16_t& port) const
    {
      sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      addr.sin_port = htons(port);
      socklen_t len = sizeof(addr);

      if((connect(_fd,(struct sockaddr*)&addr,len))<0)
      {
        printf("connect!\n");
        return false;
      }
      return true;
    }
    //获取文件描述符
    int GetFd()
    {
      return _fd;
    }
  private:
    int _fd;
};
