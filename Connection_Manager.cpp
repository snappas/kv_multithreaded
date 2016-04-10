//
// Created by ryan on 2/16/16.
//
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#include <unistd.h>
#include <thread>
#include <sstream>
#include "Connection_Manager.hpp"
#include "String_Utilities.hpp"

Connection_Manager::Connection_Manager(int _peer_fd, std::shared_ptr<KV_Manager> _kv) {
  peer_fd = _peer_fd;
  socketIsActive = true;
  kv = _kv;
  incoming_message.reserve(4096);
  outgoing_message.reserve(4096);
}


bool Connection_Manager::read_socket_data() {
  size_t bufferSize = 1;
  char buffer;
  ssize_t bytes_received = 0;
  std::string message;
  message.reserve(4096);
  try {
    while ((bytes_received = read(peer_fd, &buffer, bufferSize)) > 0) {
      if (buffer == '\n') {
        message.push_back(buffer);
        break;
      } else {
        message.push_back(buffer);
      }
      std::memset(&buffer, 0, bufferSize);
    }
    if (bytes_received < 1) {
      throw std::length_error(std::to_string(bytes_received));
    }
  } catch (std::exception &e) {
    std::cout << "Socket Error: read return value=" << e.what() << std::endl;
    close(peer_fd);
    socketIsActive = false;
    return false;
  }

  incoming_message = message;
  return true;

}

bool Connection_Manager::write_socket_data() {
  const char *socket_message = outgoing_message.c_str();
  size_t message_length = strlen(socket_message);
  try {
    while (message_length > 0) { //no guarantee all bytes will be written in 1 call
      ssize_t bytes_sent = write(peer_fd, socket_message, message_length);
      if (bytes_sent < 1) {
        throw std::length_error(std::to_string(bytes_sent));
      }
      socket_message += bytes_sent;
      message_length -= bytes_sent;
    }
  } catch (std::exception &e) {
    std::cout << "Socket Error: write return value=" << e.what() << std::endl;
    close(peer_fd);
    socketIsActive = false;
    return false;
  }
  return true;
}

bool Connection_Manager::process_message() {
  std::vector<std::string> argv = String_Utilities::tokenize_ss(incoming_message);
  switch (argv.size()) {
    case 3: //set
      if (argv.front().compare("set") == 0) {
        outgoing_message = kv->set_key(argv[1], argv[2]);
        return true;
      }
      break;
    case 2: //get
      if (argv.front().compare("get") == 0) {
        outgoing_message = kv->get_value(argv[1]);
        return true;
      }
      break;
    case 1: //quit
      if (argv.front().compare("quit") == 0) {
        close(peer_fd);
        socketIsActive = false;
      }
      if (argv.front().compare("kill") == 0) {
        close(peer_fd);
        socketIsActive = false;
        exit(0);
      }
      break;
    default:
      outgoing_message = "";
      break;

  }
  return false;
}

bool Connection_Manager::socket_is_active() {
  return socketIsActive;
}
