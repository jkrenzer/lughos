#ifndef BASIC_TYPES_HPP
#define BASIC_TYPES_HPP
#include <cstdlib>
#include <climits>

#include "expose.hpp"
#include "io.hpp"
#include "basicIo.hpp"

namespace lughos
{

template <> class exposedType<int> : public exposedTypeTemplate<int>
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

template <> class exposedValue<int> : public exposedValueTemplate<int>
{
public:
  bool verify(int value)
  {
    return true;
  }
  
  int stringToType(std::string input)
  {
    long int result = std::strtol(input.c_str(),NULL,0);
    if (result != 0L && result < INT_MAX && result > INT_MIN)
      return (int) result;
    else
      BOOST_THROW_EXCEPTION( exception() << errorName("invalid_value_supplied") << errorTitle("The provided data could not be transformed in a veritable value.") << errorSeverity(severity::Informative) );
  }
  
};

template <> class ioValueRenderer<consoleContext,int>
{
public:
  void input(exposedValue< int > &value, consoleContext context)
  {
    std::cout << "Enter value (int): ";
    std::string input;
    std::cin >> input;
    value.setValue(value.stringToType(input));
  }

 void output(exposedValue< int > &value, consoleContext context)
  {
    std::cout << "Value (int) is: " << value.getValue() << std::endl;
  }
};

template <> class ioValueRenderer<consoleContext,exposedTreeObject >
{
public:
  void input(exposedTreeObject &value, consoleContext context)
  {
  }

 void output(exposedTreeObject &value, consoleContext context)
  {
//     std::cout << "Name is: " << value.getName() << std::endl;
  }
};
 

} //namespace lughos
#endif