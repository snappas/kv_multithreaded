#ifndef EPOCHLABS_TEST_SERVER_HPP
#define EPOCHLABS_TEST_SERVER_HPP

#include <string>
#include <memory>
#include <mutex>
#include <map>
#include <vector>
#include <unordered_map>
#include "KV_Manager.hpp"

namespace EpochLabsTest {

class Server {
 public:
  Server(const std::string &listen_address, int listen_port);
  void run();
 private:
  int listen_fd;
  //add your members here
  std::shared_ptr<KV_Manager> kv;

  int accept_new_connection();
  void throw_error(const char *msg_, int errno_);
  //add your methods here
  void handle_connection(int peer_fd);

};

}

#endif

