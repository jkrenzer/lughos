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

#include <iostream>
#include <iomanip>
#include <string>
#include <boost/regex.hpp>

using namespace std;

int main()
{
  string str("Blah/\nBlubb/Bell\n/Gurgel");
  const int subs[] = {1};
  boost::regex r("(\n)");
  cout << "Analyzing: " << endl << str << endl;
  cout << "Found matches:" << endl;
  boost::sregex_token_iterator j;
  for(boost::sregex_token_iterator i(str.begin(), str.end(), r, subs); i !=j; i++) 
    cout << "Match: " << *i << endl;
  char buffer [50];
  sprintf (buffer, "%A",10.13);
  std::cout << "And a hex: " << buffer << std::endl;
}