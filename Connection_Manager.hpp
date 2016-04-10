#ifndef KV_CONNECTION_H
#define KV_CONNECTION_H

#include "KV_Manager.hpp"
#include <iostream>

class Connection_Manager {
 public:
  Connection_Manager() { };
  Connection_Manager(int _peer_fd, std::shared_ptr<KV_Manager> _kv);
  ~Connection_Manager() { };
  bool read_socket_data();
  bool write_socket_data();
  bool process_message();
  bool socket_is_active();
 private:
  std::shared_ptr<KV_Manager> kv;
  int peer_fd;
  bool socketIsActive;
  std::string incoming_message;
  std::string outgoing_message;
};


#endif //KV_CONNECTION_H
