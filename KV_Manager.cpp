#include <sstream>
#include "KV_Manager.hpp"

std::string KV_Manager::set_key(std::string key, std::string value) {
  std::ostringstream message;
  kv_mutex.lock();
  kv[key] = value;
  kv_mutex.unlock();
  message << key << "=" << value << "\n";
  return message.str();
}

std::string KV_Manager::get_value(std::string key) {
  std::ostringstream message;
  std::string value;
  kv_mutex.lock();
  try {
    value = kv.at(key);
  } catch (std::out_of_range) {
    value = "null";
  }
  kv_mutex.unlock();
  message << key << "=" << value << "\n";
  return message.str();
}
