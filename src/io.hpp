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

class ioRendererInterface
{
public:
    enum Options {READONLY = 1, READWRITE = 2} options;
    
};

template <class C, class T> class ioRendererImplementation : public ioRendererInterface
{
protected: 
  T& rObj;
public:
  //TODO Lateron we might relax these strong constraints to handle runtime-included objects.
  using ioRendererInterface::Options;
  
  ioRendererImplementation<C,T>() : rObj()
  {
    
  }
  
  ioRendererImplementation<C,T>(T& rObj) : rObj(rObj)
  {
    
  }
  
  T& getRenderObject()
  {
    return this->rObj;
  }
  
  void setRenderObject(T& t)
  {
    this->rObj = t;
  }

  virtual void render(Options o) = 0;
  
};

template <class C, class T> class ioRenderer : public ioRendererImplementation<C,T>
{};

class renderValueInterface
{
public:
  
  ioRendererInterface::Options options;
  
  void render(ioRendererInterface::Options o);
  
};

template <class C, class T> class renderValue : public renderValueInterface, public exposedValue<T>
{
protected:
  typedef ioRenderer<C,exposedValue<T> > Renderer;
  Renderer* renderer;
  
public:
  
  renderValue<C,T>()
  {
    this->renderer = new Renderer();
    this->renderer->setRenderObject(*this);
  }
  
  renderValue<C,T>(const exposedValue<T> &c) : exposedValue<T>(c)
  {
    this->renderer = new Renderer();
  }
  
  void setRenderer(ioRenderer<C,T>* r)
  {
    this->renderer=r;
    this->renderer->setRenderObject(*this);
  }
  
  Renderer& getRenderer()
  {
    return *(this->renderer);
  }
  
  void render(ioRendererInterface::Options o = ioRendererInterface::Options::READONLY)
  {
    if(this->renderer)
    {
      this->renderer->render(o);
    }
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