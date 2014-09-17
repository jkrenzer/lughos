#ifndef BASIC_IO_HPP
#define BASIC_IO_HPP
#include<iostream>
#include<string>
#include "io.hpp"

namespace lughos
{

class consoleContext : public ioContext
{
public:
  std::string getName()
  {
    return std::string("consoleContext");
  }
  
  std::string getDescription()
  {
    return std::string("Context for input/output of values via console interface.");
  }
  
};

}//namespace lughos
#endif