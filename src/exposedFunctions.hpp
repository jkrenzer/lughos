#ifndef EXPOSED_FUNCTIONS_HPP
#define EXPOSED_FUNCTIONS_HPP

#include <boost/function.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/regex.hpp>
#include <vector>

#include "exposedValues.hpp"
#include "values.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/invoke.hpp>

namespace lughos
{

  template <class ReturnType, typename ... Arguments> class ExposedFunction : public ExposedObject
  {
  protected:
    
    boost::function<ReturnType (Arguments...)> function;
    
  public:
    
    boost::signals2::signal<bool (Arguments&...), allSlotsTrue> beforeExecute;
    boost::signals2::signal<bool (ReturnType&), allSlotsTrue> afterExecute;
    boost::signals2::signal<void (ReturnType&)> onExecute;
    
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
       
    ReturnType operator()(Arguments... arguments)
    {
      if(function)
      {
	ReturnType returnValue;
	if (beforeExecute(arguments...))
	  returnValue = this->function(arguments...);
	else
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function was not called because a pre-processor deemed an argument or precondition invalid.") << errorDescription("This error occurs, when a exposed function is called and one of the pre-processors returns \"false\". This could be, for example, if a prerequisit for execution of this function is not met.") << errorSeverity(severity::ShouldNot) );
	if(!afterExecute(returnValue))
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function return-value value was deemed inacceptable by a postprocessor.") << errorDescription("This error occurs, when a exposed function is called and finished but the returned value causes one of the post-processors to return \"false\".") << errorSeverity(severity::ShouldNot) );
	onExecute(returnValue);
	return returnValue;
      }
      else
	BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_not_runnable") << errorTitle("An exposed function was called but it is not executable") << errorDescription("This error occurs, when a exposed function is called but runable-state-verification-method reports an inexecutable state of the function object. This can be the case when arguments are missing or of wrong type.") << errorSeverity(severity::ShouldNot) );
    }
    
    std::string parse(std::string command)
    {
      boost::fusion::vector<Arguments...> argV;
      ReturnType returnValue;
      if(boost::spirit::qi::phrase_parse(arguments.cbegin(),arguments.cend(),argV,boost::spirit::qi::space))
      {
	returnValue = boost::fusion::invoke(this->function,argV);
	//TODO Value to string
      }
    }
    
  };

}
#endif
