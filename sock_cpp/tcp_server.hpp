/*
 *这是一个通用tcp服务器
 */
#pragma once
#include"tcp_socket.hpp"
#include<functional>
#include<string>
//回应处理的仿函数
typedef std::function<void (const std::string& req,std::string* resp)> Handler;


class Tcp_server
{
  public:
    Tcp_server(const std::string& ip,const uint16_t port)
      :_ip(ip)
       ,_port(port)
  {}
    bool start(Handler handler)
    {
      //1.创建套接字并检验正确性
      CHECK_RET(listen_sock.Socket());
      //2.为套接字绑定ip地址，端口号
      CHECK_RET(listen_sock.Bind(_ip,_port));
      //3.改变套接字的状态为监听状态
      CHECK_RET(listen_sock.Listen());
      //4.进入循环事件
      while(1)
      {
        //accept
        Tcp_socket client_sock;
        std::string cli_ip;
        uint16_t cli_port;
        if (!listen_sock.Accept(&client_sock,&cli_ip,&cli_port))
          continue;
        printf("cleint :[%s] [%d] get link!\n",cli_ip.c_str(),cli_port);
        //5.循环读写（发送，接收）
        while(1)
        {
          std::string buf;
          //6.接收消息
          if(!listen_sock.Recv(&buf))
          {
            printf("client[%s][%d] close!\n",cli_ip.c_str(),cli_port);
            client_sock.Close();
            break;
          }
          //7.计算响应
          std:: string resp;
          handler(buf,&resp);
          //8.响应
          listen_sock.Send(resp);
        }  
      }
      return true;
    }
      private:
      Tcp_socket listen_sock;
      std::string _ip;
      uint16_t _port;
};

