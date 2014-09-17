#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP
#include <cstdlib>
#include <sstream>
#include <climits>

#include "exposedValues.hpp"
#include "io.hpp"
#include "basicIo.hpp"
#include "treeObj.hpp"
#include "transformations.hpp"

namespace lughos
{

template <> class exposedValue<int> : public exposedTypeTemplate<int>
{
public:
  std::string getGlobalTypeName()
  {
    return std::string("int");
  }
  
  std::string getTypeDescription()
  {
    return std::string("Simple, standard integer value.");
  }
  
};

template <> class exposedValue<std::string> : public exposedTypeTemplate<std::string>
{
public:
  std::string getGlobalTypeName()
  {
    return std::string("string");
  }
  
  std::string getTypeDescription()
  {
    return std::string("Simple, standard string value.");
  }
  
};


template <> class transformation<std::string>
{
public:
  
  template <class T> const std::string operator()(const exposedValue<T> &e)
  {
    return this->operator()(e.getValue());
  }
  
  const std::string operator()(const int &i)
  {
    std::stringstream ss;
    ss << "Integer: " << i;
    return ss.str();
  }
  
  const std::string operator()(const std::string &s)
  {
    return s;
  }
    
};




 

} //namespace lughos
#endif