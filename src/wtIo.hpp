#ifndef WT_IO_HPP
#define WT_IO_HPP
#include "basicIo.hpp"
#include <Wt/WLineEdit>

namespace lughos 
{
  
  class wtContext : public ioContext
{
public:
  std::string getName()
  {
    return std::string("wtContext");
  }
  
  std::string getDescription()
  {
    return std::string("Context for Wt GUI-Interface.");
  }
  
};

  
template <> class ioWrapper<wtContext>
{
  
  virtual Wt::WWidget* output(Wt::WContainerWidget* parent) = 0;
  
};

template <> class textLine<wtContext> : public ioWrapper<wtContext>
  {
  protected:
    std::string text;
    Wt::WLineEdit* lineEdit;
    exposedValueInterface* object;
    
    template <class T> void callbackTemplate(textLine<wtContext>* objPtr)
    {
      exposedValue<T>* e = dynamic_cast<exposedValue<T>*>(objPtr->object);
      if (e)
      {
	std::string s = objPtr->lineEdit->text().toUTF8();
	T value = boost::lexical_cast<T>(s);
	std::cout << "Got value " << value << " of type " << typeid(value).name() << std::endl;
	e->setValue(value);
      }
      else
	std::cout << "Could not cast. :/" << std::endl;
      
    }
    
    void (textLine<wtContext>::*callback)(textLine<wtContext>*);
    
  public:
    template <class T> textLine(exposedValue<T> &e) 
    {
      this->lineEdit = NULL;
      this->text = boost::lexical_cast<std::string>(e.getValue());
      this->object = dynamic_cast<exposedValueInterface*>(&e);
      this->callback = &textLine< wtContext >::callbackTemplate<T>;
    }
    
    Wt::WWidget* output(Wt::WContainerWidget* parent)
    {
      if(this->lineEdit == NULL)
	this->lineEdit = new Wt::WLineEdit(parent);
      this->lineEdit->setText(this->text);
      return this->lineEdit;
    }
    
    void input(std::string s)
    {
      (this->*callback)(this);
    }
    
  };
} //namespace lughos
#endif