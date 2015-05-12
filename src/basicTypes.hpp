#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP
#include <cstdlib>
#include <sstream>
#include <climits>
#include <type_traits>

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

template <> class Type<long int> : public TypeImplementation<int>
{
public:
  std::string getName()
  {
    return std::string("long int");
  }
  
  std::string getShortDescription()
  {
    return std::string("Simple, long integer value.");
  }
  
  std::string getDescription()
  {
    return std::string("Simple, long integer value.");
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

//Enums *sigh*...

template <class T> class TypeImplementation<T, typename std::enable_if<std::is_enum<T>::value>::type> : public TypeInterface
{
public:
  
  virtual bool verify(T value) = 0;

  constexpr typename std::underlying_type<T>::type to_underlying(T t) 
  {
    return static_cast<typename std::underlying_type<T>::type>(t);
  }
  
  virtual std::string toString(T t)
  {
    std::stringstream ss;
    ss << to_underlying(t);
    return ss.str();
  }
  
  virtual T fromString(std::string str)
  {
    std::stringstream ss(str);
    typename std::underlying_type<T>::type t;
    ss >> t;
    return static_cast<T>(t);
  }
  
};

template <class T> class Type<T, typename std::enable_if<std::is_enum<T>::value>::type> : public TypeImplementation<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
  public:
  std::string getName()
  {
    return std::string("enum");
  }
  
  std::string getShortDescription()
  {
    return std::string("Silly old enum.");
  }
  
  std::string getDescription()
  {
    return std::string("Silly old enum.");
  }
  
  bool verify(T value)
  {
    return true;
  }
};

} //namespace lughos
#endif