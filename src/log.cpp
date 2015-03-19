#include "log.hpp"
#include <boost/thread/thread.hpp>

namespace lughos
{

    boost::mutex soutMutex;
    boost::mutex logMutex;
    soutObj sout;
    
  
  void debugLogImpl(std::string functionName, boost::filesystem::path filePath, long int lineNumber, std::string message, double severity)
  {
      //TODO Implement simultanious output to FILE, DB and CERR
      boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
      
      std::stringstream ss;
      ss << "<entry time=\"" << boost::posix_time::to_iso_extended_string(now) <<  "\" severity=\"" << severity << "\" thread=\"" << boost::this_thread::get_id() << "\" function=\"" << functionName << "\" file=\"" << filePath.filename() << "\" line=\"" << lineNumber << "\">" << std::endl << message << std::endl << "</entry>" << std::endl;
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