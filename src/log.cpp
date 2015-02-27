#include "log.hpp"

namespace lughos
{

    boost::mutex soutMutex;
    boost::mutex logMutex;
    soutObj sout;
    
  
  void debugLogImpl(std::string functionName, std::string fileName, long int lineNumber, std::string message, double severity)
  {
      //TODO Implement simultanious output to FILE, DB and CERR
      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
      std::stringstream ss;
      ss << "[" << boost::posix_time::to_iso_extended_string(now) <<  "] <" << severity << "> (" << functionName << "@" << fileName << ":" << lineNumber << " - " << message << std::endl;
      std::string logMessage = ss.str();
      if (severity >= DEBUG_THRESHOLD_CERR)
	lughos::sout << logMessage;
  }

  template <class T> lughos::soutObj& lughos::soutObj::operator<<(T val)
  {
      lughos::soutMutex.lock();
      std::cout << val;
      lughos::soutMutex.unlock();
      return *this;
  }

  lughos::soutObj& lughos::soutObj::operator<< (std::ostream& (*pfun) (std::ostream&))
  {
	  pfun(std::cout);
	  return *this;

  }

  
}