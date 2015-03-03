#ifndef EXPOSED_FUNCTIONS_HPP
#define EXPOSED_FUNCTIONS_HPP

#include <boost/function.hpp>
#include <vector>

#include "exposedValues.hpp"

namespace lughos
{

  template <class ReturnType, typename ... Arguments> class ExposedFunction : public ExposedObject
  {
  protected:
    
    boost::function<ReturnType (Arguments...)> function;
    std::vector<boost::function<bool (Arguments& ...)> > preProcessors;
    std::vector<boost::function<void (void)> > preSignals;
    std::vector<boost::function<bool (ReturnType&)> > postProcessors;
    std::vector<boost::function<void (void)> > postSignals;
    
    virtual void setDefaultCallbacks()
    {
    }

  public:
    
    ExposedFunction(boost::function<ReturnType (Arguments...)> function, std::string name, std::string description = "") : function(function)
    {
    }
    
    ~ExposedFunction()
    {
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ss << "ExposedFunction: " << getTypeDeclaration(ReturnType()).getTypeName();
    }
    
    void addPreExecutionCall(boost::function<bool (Arguments& ...)> f)
    {
      this->preProcessors.push_back(f);
    }
    
    void addPreExecutionCall(boost::function<void (void)> f)
    {
      this->preSignals.push_back(f);
    }
    
    void addPostExecutionCall(boost::function<bool (ReturnType&)> f)
    {
      this->postProcessors.push_back(f);
    }
    
    void addPostExecutionCall(boost::function<void (void)> f)
    {
      this->postSignals.push_back(f);
    }
    
    void resetCallbacks()
    {
      this->preProcessors.clear();
      this->preSignals.clear();
      this->postProcessors.clear();
      this->postSignals.clear();
      this->setDefaultCallbacks();
    }
    
    bool runCallbacks(Arguments&... arguments)
    {
      try
      {
	for(typename std::vector<boost::function<bool (Arguments& ...)> >::iterator it = preProcessors.begin(); it != preProcessors.end(); it++)
	  if(!(*it)(arguments...)) return false;
	for(typename std::vector<boost::function<void (void)> >::iterator it = preSignals.begin(); it != preSignals.end(); it++)
	  (*it)();
      }
      catch(...)
      {
	return false;
      }
      return true;
    }
    
    bool runCallbacks(ReturnType &returnValue)
    {
      try
      {
	for(typename std::vector<boost::function<bool (ReturnType&)> >::iterator it = postProcessors.begin(); it != postProcessors.end(); it++)
	  if(!(*it)(returnValue)) return false;
	for(typename std::vector<boost::function<void (void)> >::iterator it = postSignals.begin(); it != postSignals.end(); it++)
	  (*it)();
      }
      catch(...)
      {
	return false;
      }
      return true;
    }
    
    ReturnType operator()(Arguments... arguments)
    {
      if(function)
      {
	ReturnType returnValue;
	if (runCallbacks(arguments...))
	  returnValue = this->function(arguments...);
	else
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function was not called because a pre-processor deemed an argument or precondition invalid.") << errorDescription("This error occurs, when a exposed function is called and one of the pre-processors returns \"false\". This could be, for example, if a prerequisit for execution of this function is not met.") << errorSeverity(severity::ShouldNot) );
	if(!runCallbacks(returnValue))
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function return-value value was deemed inacceptable by a postprocessor.") << errorDescription("This error occurs, when a exposed function is called and finished but the returned value causes one of the post-processors to return \"false\".") << errorSeverity(severity::ShouldNot) );
	return returnValue;
      }
      else
	BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_not_runnable") << errorTitle("An exposed function was called but it is not executable") << errorDescription("This error occurs, when a exposed function is called but runable-state-verification-method reports an inexecutable state of the function object. This can be the case when arguments are missing or of wrong type.") << errorSeverity(severity::ShouldNot) );
    }
    
  };

}
#endif
