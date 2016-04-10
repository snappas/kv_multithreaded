#include "server.hpp"
#include "Connection_Manager.hpp"

#include <cstring>
#include <algorithm>

#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <sstream>

namespace EpochLabsTest {

Server::Server(const std::string &listen_address, int listen_port)
    : listen_fd(-1) {
  std::cout << "creating server" << std::endl;

  sockaddr_in listen_sockaddr_in;
  std::memset(&listen_sockaddr_in, 0, sizeof(listen_sockaddr_in));
  listen_sockaddr_in.sin_family = AF_INET;
  inet_aton(listen_address.c_str(), &listen_sockaddr_in.sin_addr);
  listen_sockaddr_in.sin_port = htons(listen_port);

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    throw_error("could not create socket", errno);
  }

  int t = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t))) {
    throw_error("could not set SO_REUSEADDR", errno);
  }

  if (bind(listen_fd, (struct sockaddr *) &listen_sockaddr_in, sizeof(listen_sockaddr_in))) {
    throw_error("could not bind listen socket", errno);
  }

  if (listen(listen_fd, 48)) {
    throw_error("could not listen on socket", errno);
  }

  //picked up by test_server.py to know server is ready
  //this line must be output after listen returns successfully
  std::cout << "listening on " << listen_address << ":" << listen_port << std::endl;
}

int Server::accept_new_connection() {
  sockaddr_in peer_addr;
  socklen_t peer_addr_size = sizeof(peer_addr);
  std::memset(&peer_addr, 0, peer_addr_size);

  //peer_fd is the file descriptor for the socket of the newly connected client
  int peer_fd = accept4(listen_fd, (struct sockaddr *) &peer_addr, &peer_addr_size, SOCK_CLOEXEC);

  if (peer_fd < 0) {
    throw_error("error accepting connection", errno);
  }

  std::cout << "accepted connection, peer_fd=" << peer_fd << std::endl;

  return peer_fd;
}

void Server::throw_error(const char *msg_, int errno_) {
  std::string msg = msg_ + std::string(" errno=") + std::to_string(errno_);
  throw std::runtime_error(msg);
}

void Server::run() {
  std::cout << "running ..." << std::endl;
  kv.reset(new KV_Manager());
  while (true) {
    std::thread request(&Server::handle_connection, this, accept_new_connection());
    request.detach();
  }
}

/*
 * Thread entry point
 */
void Server::handle_connection(int peer_fd) {
  Connection_Manager peer(peer_fd, kv);
  while (peer.socket_is_active()) {
    if (peer.read_socket_data() && peer.process_message()) {
      peer.write_socket_data();
    }
  }

}

}

