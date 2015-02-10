#include "log.hpp"

namespace lughos
{

    boost::mutex soutMutex;
    boost::mutex logMutex;
    soutObj sout;
    
  void debugLog(std::string message, double severity)
  {

  }

    
  void debugLogImpl(std::string functionName, std::string fileName, long int lineNumber, std::string message, double severity)
  {
      //TODO Implement simultanious output to FILE, DB and CERR
      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
      std::stringstream ss;
      ss << "[" << boost::posix_time::to_iso_extended_string(now) <<  "] <" << severity << "> (" << functionName << "@" << fileName << ":" << lineNumber << " - " << message << std::endl;
      std::string logMessage = ss.str();
      sout << logMessage;
  }

  soutObj& soutObj::operator<<(T val)
  {
      soutMutex.lock();
      std::cout << val;
      soutMutex.unlock();
      return *this;
  }

  lughos::soutObj& soutObj::operator<< (std::ostream& (*pfun) (std::ostream&))
  {
	  pfun(std::cout);
	  return *this;

  }

  
}