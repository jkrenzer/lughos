// stringstream::str
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf
#include <bitset>

int main () {
  std::stringstream ss;
  ss.str ("Example string");
  std::string s = ss.str();
  std::cout << "1:" << s << '\n';
  s.clear();
  s = ss.str();
  std::cout << "2:" << s << '\n';
  std::bitset<8> bs;
  std::cout << bs.to_string() << " = " << bs.to_ulong() << std::endl;
  bs[0] = true;
  std::cout << bs.to_string() << " = " << bs.to_ulong() << std::endl;
  return 0;
}