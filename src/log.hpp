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
  
  boost::mutex soutMutex;
  boost::mutex logMutex;

  /** 
  * @class soutObj
  * @brief An ostream-class which uses boost-mutex for threadsave output to cout.
  */

  class soutObj
  {
    public:


    template <class T>
    soutObj& operator<< (T val)
    {
      soutMutex.lock();
      std::cout << val;
      soutMutex.unlock();
      return *this;
    }
    
    soutObj& operator<< (std::ostream& (*pfun) (std::ostream&))
    {
	pfun(std::cout);
	return *this;
    }
  };

  soutObj sout;
  
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
  
  void debugLog(std::string message, double severity = 1.0)
  {
    
  }
  
  
  
  void debugLogImpl(std::string functionName, std::string fileName, long int lineNumber, std::string message, double severity = 1.0)
  {
    //TODO Implement simultanious output to FILE, DB and CERR
    boost::posix_time::ptime now = boost::date_time::microsec_clock::local_time();
    std::stringstream ss;
    ss << "[" << boost::posix_time::to_iso_extended_string(now) <<  "] <" << severity << "> (" << functionName << "@" << fileName << ":" << lineNumber << " - " << message << std::endl;
    std::string logMessage = ss.str();
    sout << logMessage;
  }
  
  #undef debugLog
  #define debugLog(x) debugLogImpl(__FUNCTION__, __FILE__, __LINE__,x)
  
} //namespace lughos

#endif