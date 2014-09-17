#ifndef EXPOSED_FUNCTIONS_HPP
#define EXPOSED_FUNCTIONS_HPP
#include "exposedValues.hpp"

template <class R> class exposedFunction : public exposedObject
{

public:
  
    exposedFunction(std::string name, std::string description = "") : exposedObject(name,description)
    {
    }
    
    ~exposedFunction()
    {
    }
  
  template <class T> T getValue(treeObject& childRef)
  {
    exposedValue<T>* child = (exposedValue<T>*) childRef;
    return child->getValue();
  }
  
  template <class T> T getValue(std::string childName)
  {
    exposedValue<T>* child = (exposedValue<T>*) this->getChild(childName);
    return child->getValue();
  }
  
  virtual R exec() = 0;
  
  virtual bool runable() = 0;
  
  R run()
  {
    if(this->runable())
      return this->exec();
    else
      BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_not_runnable") << errorTitle("An exposed function was called but it is not executable") << errorDescription("This error occurs, when a exposed function is called but runable-state-verification-method reports an inexecutable state of the function object. This can be the case when arguments are missing or of wrong type.") << errorSeverity(severity::ShouldNot) );
  }
  
};
#endif
