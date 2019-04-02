#include"udp_client.hpp"

int main(int argc,char* argv[])
{
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  UdpClient client(ip,port);
  std::string str = "hello";
  std::string resp;
  for(;;)
  {
    client.SendTo(str);
    client.RecvFrom(&resp);
    printf("server say:[%s]\n",resp.c_str());
    sleep(10);
  }
  return 0;
}
