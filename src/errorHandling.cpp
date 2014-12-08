#include "errorHandling.hpp"

namespace lughos
{

std::string severityToString(severity s)
{
    switch (s)
    {
      case severity::Neglectable:
	return std::string("Neglectable");
      case severity::Informative:
	return std::string("Informative");
      case severity::ShouldNot:
	return std::string("ShouldNot");
      case severity::MustNot:
	return std::string("MustNot");
      case severity::Fatal:
	return std::string("MustNot");
      case severity::Catastrophic:
	return std::string("Catastrophic");
      default:
	return std::string("Undefined");
    }
}

std::string makeErrorReport(exception& e)
{
  std::stringstream s;
  s << std::endl << std::endl;
  s << "-------------------- Error Report --------------------" << std::endl;
  if( std::string const * name=boost::get_error_info<errorName>(e) )
    s << "Name: " << *name << std::endl;
  if( severity const * sev=boost::get_error_info<errorSeverity>(e) )
    s << "Severity: " << severityToString(*sev) << std::endl;
  if( std::string const * title=boost::get_error_info<errorTitle>(e) )
    s << "Title: " << *title << std::endl;
  if( std::string const * desc=boost::get_error_info<errorDescription>(e) )
    s << "Description: " << *desc << std::endl;
  s << "---------------- Diagnostic Information --------------" << std::endl;
  if( const char** func=boost::get_error_info<boost::throw_function>(e) )
    s << "Function: " << *func << std::endl;
  if( const char** file=boost::get_error_info<boost::throw_file>(e) )
    s << "File: " << *file << std::endl;
  if( int const * line=boost::get_error_info<boost::throw_line>(e) )
    s << "Line: " << *line << std::endl;
  s << "------------------------------------------------------" << std::endl << std::endl;
  return s.str();
  
}

}//namespace lughos