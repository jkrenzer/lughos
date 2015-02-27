#ifndef IO_HPP
#define IO_HPP
#include<iostream>
#include<string>
#include<typeinfo>
#include<boost/smart_ptr/shared_ptr.hpp>

#include "values.hpp"
#include "exposedValues.hpp"
#include "errorHandling.hpp"

namespace lughos
{

  class ioContext
  {};
  
  template <class C> class ioWrapper
  {
  public:
    
    template <class O> O output()
    {
      
    }
    
    template <class O> void input(O o)
    {
      
    }
    
  };
  
  template <class C> class textLine : public ioWrapper<C>
  {
    
  };
  
  
  template <class C, class T> class defaultWrapper : textLine<C>
  {};
  
  template <class C> class ioRenderer
  {
  public:
    template <class T> void output(Value<T>& t)
    {
      std::cout << t.getValue() << std::endl;
    }    
    
    template <class T> void output(ExposedValue<T>& eV)
    {
      std::cout << "-= Object " << eV.getName() << "=-" << std::endl;
      for (int i = 0; i < eV.countChildren(); i++)
      {
// 	std::cout << eV->getName() << " - " << eV.getAs<ExposedValue>(i)->getValueAsString() << std::endl;
      }
    }
  };
  
  

} //namespace lughos
#endif