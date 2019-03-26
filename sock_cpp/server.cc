#include"tcp_server.hpp"
#include<stdlib.h>

//服务器响应代码实例
//服务器响应函数

void respond(const std::string& req,std::string* resp)
{
  printf("client say[%s]\n",req.c_str());
  *resp = req;
  return;
}
int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./server [ip] [port]\n");
    return 1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  Tcp_server ser(ip,port);
  ser.start(respond);
  return 0;
}
