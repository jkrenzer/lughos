#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <sstream>
#include <ostream>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <string>
#include <sstream>
#include <boost/current_function.hpp>

#define DEBUG_THRESHOLD_CERR 10.0
#define DEBUG_THRESHOLD_FILE 10.0
#define DEBUG_THRESHOLD_DB 10.0

namespace lughos 
{
  
    extern boost::mutex soutMutex;
    extern boost::mutex logMutex;
  
  /** 
  * @class soutObj
  * @brief An ostream-class which uses boost-mutex for threadsave output to cout.
  */

  class soutObj
  {
    public:


    template <class T>
    soutObj& operator<< (T val);
    
    soutObj& operator<< (std::ostream& (*pfun) (std::ostream&));
    
  };

  extern soutObj sout;
  
//   class debugLogEntry
//   {
//   public:
//     std::string functionName;
//     std::string fileName;
//     long int lineNumber;
//     std::string message;
//     
//     debugLogEntry(std::string functionName, std::string fileName, long int lineNumber, std::string message) : functionName(functionName), fileName(fileName), lineNumber(lineNumber), message(message)
//     {
//       
//     }
//     
//     std::String toString()
//     {
//       std::stringstream ss;
//       ss << this->functionName << this->fileName << this->lineNumber << this->message << std::endl;
//       return
//     }
//     
//     
//   };
  
  void debugLog(std::string message, double severity = 1.0);
  
  
  
  
  void debugLogImpl(std::string functionName, std::string fileName, long int lineNumber, std::string message, double severity = 1.0);
  
  #undef debugLog
  #define debugLog(x) debugLogImpl(__FUNCTION__, __FILE__, __LINE__,x)
  
} //namespace lughos

#endif