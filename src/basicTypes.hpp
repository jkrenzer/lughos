#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP
#include <cstdlib>
#include <sstream>
#include <climits>

#include "rawValues.hpp"
#include "io.hpp"
#include "basicIo.hpp"
#include "treeObj.hpp"
#include "transformations.hpp"

namespace lughos
{

template <> class ValueDeclaration<int>
{
public:
  std::string getTypeName()
  {
    return std::string("int");
  }
  
  std::string getTypeShortDescription()
  {
    return std::string("Simple, standard integer value.");
  }
  
  std::string getTypeDescription()
  {
    return std::string("Simple, standard integer value.");
  }
  
  bool verify(int value)
  {
    return true;
  }
  
};

template <> class ValueDeclaration<std::string>
{
public:
  std::string getTypeName()
  {
    return std::string("string");
  }
  
  std::string getTypeShortDescription()
  {
    return std::string("Simple, standard string value.");
  }
  
  std::string getTypeDescription()
  {
    return std::string("Simple, standard string value.");
  }
  
  bool verify(std::string value)
  {
    return true;
  }
    
};

template <> class ValueDeclaration<double>
{
public:
  std::string getTypeName()
  {
    return std::string("double");
  }
  
  std::string getTypeShortDescription()
  {
    return std::string("Simple, standard double value.");
  }
  
  std::string getTypeDescription()
  {
    return std::string("Simple, standard double value.");
  }
  
  bool verify(double value)
  {
    return true;
  }
    
};


} //namespace lughos
#endif