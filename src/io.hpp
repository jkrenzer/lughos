#ifndef IO_HPP
#define IO_HPP
#include<iostream>
#include<string>

#include "expose.hpp"
#include "errorHandling.hpp"

namespace exposer
{

class ioContext
{
public:
  
  virtual std::string getName() = 0;
  
  virtual std::string getDescription() = 0;
  
};

template <class T, class C> class ioValueRenderer
{
public:
  //TODO Lateron we might relax these strong constraints to handle runtime-included objects.
  void output(exposedVar<T> &value, C &context) = 0;
  
  void input(exposedVar<T> &value, C &context) = 0;
  
};

template <class C> class ioRenderer
{
protected:
  
  C context;
  
public:
  
  ioRenderer() : context()
  {
    
  }
  
  ioRenderer(C context) : context(context)
  {
    
  }
  
  template <class T> void output(exposedValue<T> &value)
  {
    ioValueRenderer<T,C>::output(value,this->context);
  }
  
  template <class T> void input(exposedValue<T> &value)
  {
    ioValueRenderer<T,C>::input(value,this->context);
  }
  
  
};

} //namespace exposer
#endif