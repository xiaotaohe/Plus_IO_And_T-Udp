#include"udp_socket.hpp"
#include<functional>
#include<assert.h>
//c式写法，函数指针
//typedef void (*handler)(const std::string& req,std::string* resp);
//c++利用仿函数 和 函数指针 lamda表达式

typedef std::function<void(const std::string& req,std::string* resp)> Handler;

class Udp_server
{
  public:
    Udp_server()
    {
        assert(sock.Socket());
    }
    ~Udp_server()
    {
      sock.Close();
    }
    bool start(const std::string& ip,uint16_t port,Handler handler)
    {
      //1.为套接字绑定ip/port
      bool ret = sock.Bind(ip,port);
      if(!ret)
        return false;
      //2.循环服务
      for(;;)
      {
        std::string _req;
        std::string _ip;
        uint16_t _port;
        bool ret = sock.RecvFrom(&_req,&_ip,&_port);
        if(!ret)
          continue;
        std::string resp;
        handler(_req,&resp);
        sock.SendTo(resp,_ip,_port);
        printf("client [%s][%d]say:%s\n",_ip.c_str(),_port,_req.c_str());
      }
      return true;
    }
  private:
    Udp_socket sock; 
};
