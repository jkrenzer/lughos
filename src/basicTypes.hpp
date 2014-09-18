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

template <> class valueDeclaration<int>
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
  
  bool verify(int value)
  {
    true;
  }
  
};

template <> class valueDeclaration<std::string>
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
  
  bool verify(int value)
  {
    true;
  }
    
};

template <class T> class ioRenderer<consoleContext,renderValue<consoleContext,T> > : ioRendererImplementation<consoleContext,T>
{
public:
   
  void render(ioRendererInterface::Options o)
  { 
    std::cout << "Rendering " << this->getRenderObject().getName() << std::endl;
    if (o == ioRendererInterface::READWRITE)
    {
      T t;
      std::cout << "New value: ";
      std::cin >> t;
      this->getRenderObject().setValue(t);
    }
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

template <class T> class transformation<exposedValue<T> >
{
  
};




 

} //namespace lughos
#endif