#ifndef EXPOSED_FUNCTIONS_HPP
#define EXPOSED_FUNCTIONS_HPP

#include <boost/function.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/regex.hpp>
#include <vector>

#include "exposedValues.hpp"
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
      this->name = name;
      this->description = description;
    }
    
    ~ExposedFunction()
    {
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ss << "ExposedFunction: " << Type<ReturnType>().getName();
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
      if(boost::spirit::qi::phrase_parse(command.cbegin(),command.cend(),argV,boost::spirit::qi::space))
      {
	returnValue = boost::fusion::invoke(this->function,argV);
	return Type<ReturnType>().toString(returnValue);
      }
      else
	return std::string("");
    }
    
  };
  
  template <typename ... Arguments> class ExposedFunction<void,Arguments...> : public ExposedObject
  {
  protected:
    
    boost::function<void (Arguments...)> function;
    
  public:
    
    boost::signals2::signal<bool (Arguments&...), allSlotsTrue> beforeExecute;
    boost::signals2::signal<bool (void), allSlotsTrue> afterExecute;
    boost::signals2::signal<void (void)> onExecute;
    
    ExposedFunction(boost::function<void (Arguments...)> function, std::string name, std::string description = "") : function(function)
    {
      this->name = name;
      this->description = description;
    }
    
    ~ExposedFunction()
    {
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ss << "ExposedFunction: " << "void";
    }    
       
    void operator()(Arguments... arguments)
    {
      if(function)
      {
	if (beforeExecute(arguments...))
	  this->function(arguments...);
	else
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function was not called because a pre-processor deemed an argument or precondition invalid.") << errorDescription("This error occurs, when a exposed function is called and one of the pre-processors returns \"false\". This could be, for example, if a prerequisit for execution of this function is not met.") << errorSeverity(severity::ShouldNot) );
	if(!afterExecute())
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function return-value value was deemed inacceptable by a postprocessor.") << errorDescription("This error occurs, when a exposed function is called and finished but the returned value causes one of the post-processors to return \"false\".") << errorSeverity(severity::ShouldNot) );
	onExecute();
      }
      else
	BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_not_runnable") << errorTitle("An exposed function was called but it is not executable") << errorDescription("This error occurs, when a exposed function is called but runable-state-verification-method reports an inexecutable state of the function object. This can be the case when arguments are missing or of wrong type.") << errorSeverity(severity::ShouldNot) );
    }
    
    std::string parse(std::string command)
    {
      boost::fusion::vector<Arguments...> argV;
      if(boost::spirit::qi::phrase_parse(command.cbegin(),command.cend(),argV,boost::spirit::qi::space))
      {
	boost::fusion::invoke(this->function,argV);
      }
      return std::string("");
    }
    
  };
  
  template <class ReturnType> class ExposedFunction<ReturnType,void> : public ExposedObject
  {
  protected:
    
    boost::function<ReturnType (void)> function;
    
  public:
    
    boost::signals2::signal<bool (void), allSlotsTrue> beforeExecute;
    boost::signals2::signal<bool (ReturnType&), allSlotsTrue> afterExecute;
    boost::signals2::signal<void (ReturnType&)> onExecute;
    
    ExposedFunction(boost::function<ReturnType (void)> function, std::string name, std::string description = "") : function(function)
    {
      this->name = name;
      this->description = description;
    }
    
    ~ExposedFunction()
    {
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ss << "ExposedFunction: " << getTypeDeclaration(ReturnType()).getTypeName();
    }    
       
    ReturnType operator()(void)
    {
      if(function)
      {
	ReturnType returnValue;
	if (beforeExecute())
	  returnValue = this->function();
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
      ReturnType returnValue;
      returnValue = this->function();
      return Type<ReturnType>().toString(returnValue);
    }
    
  };
  
  template <> class ExposedFunction<void> : public ExposedObject
  {
  protected:
    
    boost::function<void (void)> function;
    
  public:
    
    boost::signals2::signal<bool (void), allSlotsTrue> beforeExecute;
    boost::signals2::signal<bool (void), allSlotsTrue> afterExecute;
    boost::signals2::signal<void (void)> onExecute;
    
    ExposedFunction(boost::function<void (void)> function, std::string name, std::string description = "") : function(function)
    {
      this->name = name;
      this->description = description;
    }
    
    ~ExposedFunction()
    {
    }
    
    std::string showStructure()
    {
      std::stringstream ss;
      ss << "ExposedFunction: " << "void";
    }    
       
    void operator()(void)
    {
      if(function)
      {
	if (beforeExecute())
	  this->function();
	else
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function was not called because a pre-processor deemed an argument or precondition invalid.") << errorDescription("This error occurs, when a exposed function is called and one of the pre-processors returns \"false\". This could be, for example, if a prerequisit for execution of this function is not met.") << errorSeverity(severity::ShouldNot) );
	if(!afterExecute())
	  BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_invalid_value") << errorTitle("An exposed function return-value value was deemed inacceptable by a postprocessor.") << errorDescription("This error occurs, when a exposed function is called and finished but the returned value causes one of the post-processors to return \"false\".") << errorSeverity(severity::ShouldNot) );
	onExecute();
      }
      else
	BOOST_THROW_EXCEPTION( exception() << errorName("executed_exposed_function_not_runnable") << errorTitle("An exposed function was called but it is not executable") << errorDescription("This error occurs, when a exposed function is called but runable-state-verification-method reports an inexecutable state of the function object. This can be the case when arguments are missing or of wrong type.") << errorSeverity(severity::ShouldNot) );
    }
    
    std::string parse(std::string command)
    {
      this->function();
      return std::string("");
    }
    
  };

}
#endif
