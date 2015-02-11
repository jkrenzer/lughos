#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP
#include <string>
#include <sstream>
#include <boost/exception/all.hpp>

namespace lughos
{

enum severity { Neglectable, Informative, ShouldNot, MustNot, Fatal, Catastrophic };

typedef boost::error_info<struct errorNameStruct,std::string> errorName;
typedef boost::error_info<struct errorTitleStruct,std::string> errorTitle;
typedef boost::error_info<struct errorDrescriptionStruct,std::string> errorDescription;
typedef boost::error_info<struct errorSeverityStruct,severity> errorSeverity;

struct exception: virtual boost::exception, virtual std::exception { };




} //namespace lughos
#endif