#ifndef EXPOSED_FUNCTIONS_HPP
#define EXPOSED_FUNCTIONS_HPP

#include <boost/function.hpp>

#include "exposedValues.hpp"

namespace lughos
{

  template <class ReturnType, typename Arguments...> class ExposedFunction : public ExposedObject
  {
  protected:
    
    boost::function<ReturnType (Arguments...)> function;

  public:
    
      exposedFunction(std::string name, std::string description = "") : exposedObject(name,description)
      {
      }
      
      ~exposedFunction()
      {
      }
    
    template <class T> T getValue(TreeObject& childRef)
    {
      Value<T>* child = (Value<T>*) childRef;
      return child->getValue();
    }
    
    template <class T> T getValue(std::string childName)
    {
      Value<T>* child = (Value<T>*) this->getChild(childName);
      return child->getValue();
    }
    
    ReturnType operator()(Arguments... arguments)
    {
      if(function)
	return this->function(arguments);
      else
	BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_not_runnable") << errorTitle("An exposed function was called but it is not executable") << errorDescription("This error occurs, when a exposed function is called but runable-state-verification-method reports an inexecutable state of the function object. This can be the case when arguments are missing or of wrong type.") << errorSeverity(severity::ShouldNot) );
    }
    
  };

}
#endif
