/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

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