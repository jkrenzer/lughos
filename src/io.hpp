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
    
  virtual T& input(T &value, C &context) =0;
  
};

template <class C, class T> class ioValueRenderer : public ioValueRendererImpl<C,T>
{};

class renderValueInterface
{
public:
  
  virtual void input() = 0;
  
  virtual void output() = 0;
  
  
};

template <class C, class T> class renderValue : public renderValueInterface, public exposedValue<T>
{
protected:
  ioValueRenderer<C,T>* renderer;
  
public:
  
  renderValue<C,T>() : renderer()
  {
    this->renderer = NULL;
  }
  
  renderValue<C,T>(const exposedValue<T> &c) : exposedValue<T>(c), renderer()
  {
    this->renderer = NULL;
  }
  
  void setRenderer(ioValueRenderer<C,T>* r)
  {
    this->renderer=r;
  }
  
  ioValueRenderer<C,T>* getRenderer()
  {
    return this->renderer;
  }
  
  void input()
  {
    if(this->renderer)
      this->renderer->input(*this,C() );
  }
  
  void input(C context)
  {
    if(this->renderer)
      this->renderer->input(*this,context );
  }
  
  void output()
  {
    if(this->renderer)
      this->renderer->output(*this,C() );
  }
  
  void output(C context)
  {
    if(this->renderer)
      this->renderer->output(*this,context );
  }
  
};

template <class K> class formField
{
  template <class C, class T> void render(renderValue<C,T> obj)
  {
    std::cout << "No formfield for this type defined." << std::endl;
  }
};

} //namespace lughos
#endif