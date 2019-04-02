#include"udp_server.hpp"
void handler(const std::string& req,std::string* resp)
{
  resp->assign(req.c_str(),req.size());
  printf("handler -> resp = %s\n",(*resp).c_str());
  return;
}
int main(int argc,char* argv[])
{
  if(argc<3)
  {
    printf("./server [ip] [port]\n");
    return 1;
  }
  Udp_server server;
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  server.start(ip,port,handler);
}
