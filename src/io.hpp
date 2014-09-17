#ifndef IO_HPP
#define IO_HPP
#include<iostream>
#include<string>
#include<typeinfo>

#include "expose.hpp"
#include "errorHandling.hpp"

namespace lughos
{

class ioContext
{
public:
  
  virtual std::string getName() = 0;
  
  virtual std::string getDescription() = 0;
  
};

class rendererImpl
{
  virtual void output() = 0;
  
  virtual void input() = 0;
};

template <class C, class T> class ioValueRendererImpl
{
public:
  //TODO Lateron we might relax these strong constraints to handle runtime-included objects.
  virtual void output(T &value, C &context) =0;
    
  virtual void input(T &value, C &context) =0;
  
};

template <class C, class T> class ioValueRenderer : public ioValueRendererImpl<C,T>
{};

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
  
  
  template <class T> void output(T &value)
  {
    ioValueRenderer<C,T> renderer;
    renderer.output(value,this->context);
  }
  
  template <template <class> class W, class T > void output(W<T> &value)
  {
    ioValueRenderer<C,T> renderer;
    renderer.output(value,this->context);
  }
  
  
  template <class T> void input(T &value)
  {
    ioValueRenderer<C,T> renderer;
    renderer.input(value,this->context);
  }
  
  template <template <class> class W, class T > void input(W<T> &value)
  {
    ioValueRenderer<C,T> renderer;
    renderer.input(value,this->context);
  }
  
};

} //namespace lughos
#endif