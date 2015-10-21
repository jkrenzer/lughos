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
#include <boost/smart_ptr/shared_ptr.hpp>
using namespace std;

class Var
{
public:
  int value;
  
  Var()
  {
    value = -100;
  }
  
  void plus100()
  {
    value += 100;
  }
};

class A
{
public:
  int A;
  
  virtual std::string foo() = 0;
};

class B : public A
{
public:
  std::string B;
  Var v;
  
  std::string foo()
  {
    v.plus100();
    return B + std::string(to_string(v.value));
  }
  
};

int main()
{
  boost::shared_ptr<B> b(new B);
  b->B = std::string("Ich foo hier herum!");
  cout << b->foo() << endl;
  b->B = std::string("Ich foo nochmal!");
  boost::shared_ptr<A> a;
  a= b;
  cout << a->foo() << endl;
  a = boost::shared_ptr<A>(b);
  cout << a->foo() << endl;
  return 0;
}