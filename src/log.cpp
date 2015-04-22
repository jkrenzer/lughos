#include "log.hpp"
#include <boost/thread/thread.hpp>
#include <boost/thread/lock_guard.hpp>

namespace lughos
{

    
    boost::mutex logMutex;
    soutObj sout;
    
  
  void debugLogImpl(std::string functionName, boost::filesystem::path filePath, long int lineNumber, std::string message, double severity)
  {
      //TODO Implement simultanious output to FILE, DB and CERR
      boost::lock_guard<boost::mutex> lock(logMutex);
      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
      
      std::stringstream ss;
      ss << "<entry time=\"" << boost::posix_time::to_iso_extended_string(now) <<  "\" severity=\"" << severity << "\" thread=\"" << boost::this_thread::get_id() << "\" function=\"" << functionName << "\" file=\"" << filePath.filename() << "\" line=\"" << lineNumber << "\">" << std::endl << message << std::endl << "</entry>" << std::endl;
      std::string logMessage = ss.str();
      if (severity >= DEBUG_THRESHOLD_CERR)
	lughos::sout << logMessage;
  }

  template <class T> lughos::soutObj& lughos::soutObj::operator<<(T val)
  {
    boost::lock_guard<boost::mutex> lock(soutMutex);
      std::cout << val;
      return *this;
  }

  lughos::soutObj& lughos::soutObj::operator<< (std::ostream& (*pfun) (std::ostream&))
  {
    boost::lock_guard<boost::mutex> lock(soutMutex);
    pfun(std::cout);
    return *this;

  }
  
  boost::mutex lughos::soutObj::soutMutex;

  
}