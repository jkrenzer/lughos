#ifndef GUI_FORMS_HPP
#define GUI_FORMS_HPP
#include "lughosObjects.hpp"
#include "io.hpp"
#include "expose.hpp"

namespace lughos
{

class wtContext
{
};

class formImpl : public configurableObject
{
 public:
    virtual void render() = 0; 
};

template <class C> class formTemplate : public exposedTreeObject, public configurableObject
{
protected:
  ioRenderer<C> renderer;
  
public:
  virtual void render() = 0;
  
    formTemplate() : renderer()
    {
    }
};

template <class C> class form : public formTemplate<C>
{
  virtual void renderMe()
  {
  }
  
  void renderChildren()
  {
    this->renderChildren(this);
  }
  
  virtual void renderChildren(exposedTreeObject* tObj)
  {
    std::vector<exposedTreeObject*> childrenV = tObj->getChildren();
    std::cout << "Es sind " << childrenV.size() << " Kindelemente anzuzeigen." << std::endl;
    for (std::vector<exposedTreeObject*>::iterator it = childrenV.begin(); it < childrenV.end(); ++it)
    {
      try 
      {
	this->render(*it);
      }
      catch(lughos::exception e)
      {
	    std::cerr << lughos::makeErrorReport(e);
      }
    }
  }
  
  virtual void render(exposedTreeObject* tObj)
  {
    this->renderer.output(*tObj);
    this->renderChildren(tObj);
  }
  
  
  
  template <class T> void render(exposedValue<T>* obj)
  {
    this->renderer.output(obj);
  }
  
public:
  
  virtual void render()
  {
    this->render(this);
  }
};

}//namespace lughos
#endif