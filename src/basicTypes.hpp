#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP
#include <cstdlib>
#include <sstream>
#include <climits>

#include "values.hpp"

namespace lughos
{

template <> class Type<int> : public TypeImplementation<int>
{
public:
  std::string getName()
  {
    return std::string("int");
  }
  
  std::string getShortDescription()
  {
    return std::string("Simple, standard integer value.");
  }
  
  std::string getDescription()
  {
    return std::string("Simple, standard integer value.");
  }
  
  bool verify(int value)
  {
    return true;
  }
  
};

template <> class Type<std::string>  : public TypeImplementation<std::string>
{
public:
  std::string getName()
  {
    return std::string("string");
  }
  
  std::string getShortDescription()
  {
    return std::string("Simple, standard string value.");
  }
  
  std::string getDescription()
  {
    return std::string("Simple, standard string value.");
  }
  
  bool verify(std::string value)
  {
    return true;
  }
    
};

template <> class Type<double>  : public TypeImplementation<double>
{
public:
  std::string getName()
  {
    return std::string("double");
  }
  
  std::string getShortDescription()
  {
    return std::string("Simple, standard double value.");
  }
  
  std::string getDescription()
  {
    return std::string("Simple, standard double value.");
  }
  
  bool verify(double value)
  {
    return true;
  }
    
};

template <> class Type<float>  : public TypeImplementation<float>
{
public:
  std::string getName()
  {
    return std::string("float");
  }
  
  std::string getShortDescription()
  {
    return std::string("Simple, standard float value.");
  }
  
  std::string getDescription()
  {
    return std::string("Simple, standard float value.");
  }
  
  bool verify(float value)
  {
    return true;
  }
    
};

template <> class Type<bool>  : public TypeImplementation<bool>
{
public:
  std::string getName()
  {
    return std::string("bool");
  }
  
  std::string getShortDescription()
  {
    return std::string("Simple, standard bool value.");
  }
  
  std::string getDescription()
  {
    return std::string("Simple, standard bool value.");
  }
  
  bool verify(bool value)
  {
    return true;
  }
    
};

template <size_t N> class Type<std::bitset<N> > : public TypeImplementation<std::bitset<N> >
{
public:
  std::string getName()
  {
    return std::string("bitset");
  }
  
  std::string getShortDescription()
  {
    return std::string("STD-container for bitsets.");
  }
  
  std::string getDescription()
  {
    return std::string("STD-container for bitsets.");
  }
  
  bool verify(std::bitset<N> value)
  {
    return true;
  }
};

} //namespace lughos
#endif