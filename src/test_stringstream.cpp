// stringstream::str
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

int main () {
  std::stringstream ss;
  ss.str ("Example string");
  std::string s = ss.str();
  std::cout << "1:" << s << '\n';
  s.clear();
  s = ss.str();
  std::cout << "2:" << s << '\n';
  return 0;
}