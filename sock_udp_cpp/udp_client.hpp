#pragma once
#include "udp_socket.hpp"
#include<assert.h>

class UdpClient {
  public:
    UdpClient(const std::string& ip, uint16_t port) : ip_(ip), port_(port) {
      assert(sock_.Socket());
    }
    ~UdpClient() {
      sock_.Close();
    }
    bool RecvFrom(std::string* buf) {
      return sock_.RecvFrom(buf);
    }
    bool SendTo(const std::string& buf) {
      printf("hpp : send!\n");
      return sock_.SendTo(buf, ip_, port_);
    }
  private:
    Udp_socket sock_;
    // 服务器端的 IP 和 端口号
    std::string ip_;
    uint16_t port_;
};
