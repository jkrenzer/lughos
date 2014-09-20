#ifndef IO_HPP
#define IO_HPP
#include<iostream>
#include<string>
#include<typeinfo>
#include<boost/smart_ptr/shared_ptr.hpp>

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
  
  
  
  
  template <class C> class ioRenderer
  {
  public:
    template <class T> void output(exposedValue<T>& t)
    {
      std::cout << t.getValue() << std::endl;
    }
    
  };

} //namespace lughos
#endif