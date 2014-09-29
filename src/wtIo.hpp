#ifndef WT_IO_HPP
#define WT_IO_HPP
#include "basicIo.hpp"
#include <Wt/WLineEdit>
#include <Wt/WTemplate>

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
  public:
    virtual Wt::WWidget* output() = 0;
  
};

template <> class textLine<wtContext> : public ioWrapper<wtContext>
  {
  protected:
    static Wt::WString templateString;
    Wt::WTemplate* formTemplate;
    Wt::WLineEdit* lineEdit;
    Wt::WText* label;
    ValueInterface* object;
    
    template <class T> void callbackTemplate(textLine<wtContext>* objPtr)
    {
      Value<T>* e = dynamic_cast<Value<T>*>(objPtr->object);
      if (e)
      {
	std::string s = objPtr->lineEdit->text().toUTF8();
	T value = boost::lexical_cast<T>(s);
	e->setValue(value);
      }
      else
	std::cout << "Could not cast. :/" << std::endl;
      
    }
    
    void (textLine<wtContext>::*callback)(textLine<wtContext>*);
    
  public:
    template <class T> textLine(Value<T> &e)
    {
      this->formTemplate = new Wt::WTemplate(textLine< wtContext >::templateString);
      this->lineEdit = new Wt::WLineEdit;
      this->label = new Wt::WText;
      this->lineEdit->setText(boost::lexical_cast<std::string>(e.getValue()));
      this->label->setText(e.getName());
      this->object = dynamic_cast<ValueInterface*>(&e);
      this->callback = &textLine< wtContext >::callbackTemplate<T>;
      this->formTemplate->bindWidget("label",this->label);
      this->formTemplate->bindWidget("field",this->lineEdit);      
    }
    
    Wt::WWidget* output()
    {
      return this->formTemplate;
    }
    
    void input(std::string s)
    {
      (this->*callback)(this);
    }
    
  };
  
  /**
   * @brief HTML-template for textLine objects.
   * 
   */
  Wt::WString textLine<wtContext>::templateString(
    "\
    <div class=\"form-inline\">\
      <div class=\"form-group\">\
	${label}\
      </div>\
      <div class=\"form-group\">\
	${field}\
      </div>\
    </div>");
  
} //namespace lughos
#endif