#ifndef QUERY_HPP
#define QUERY_HPP
#include "threadSafety.hpp"
#include "errorHandling.hpp"
#include "log.hpp"
#include <boost/assert.hpp>
#include <boost/chrono.hpp>
#include <boost/regex.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/locks.hpp>
#include <boost/asio/streambuf.hpp>


namespace lughos
{
  //TODO add continous answer retrival
  
  //TODO add casts so queries can be used directly with asio-functions.
  

  class QueryImpl
  {
  protected:
    bool sent;
    bool awaitingAnswer;
    bool continous; 
    bool done;
    bool error;
    
    boost::signals2::signal<void (std::vector<std::string>&)> onReceived;
    boost::signals2::signal<void (void)> onSent;
    
    std::string question;
    boost::shared_ptr<boost::shared_future<std::string> > answer;
    boost::shared_ptr< boost::promise< std::string > > promise;
    boost::shared_ptr<boost::asio::streambuf> response;
    boost::shared_ptr<boost::asio::streambuf> request;
    boost::shared_ptr<boost::unique_lock<boost::mutex> > busyLock;
    
    boost::shared_mutex mutex;
    
    boost::regex EOLpattern;
    
    std::string lastErrorMessage;
    
    
  public:
    
    QueryImpl(std::string question)
    {
      this->purge();
      this->setQuestion(question);
    }
    
    ~QueryImpl()
    {

    }
    
    boost::regex getEOLPattern()
    {
      lughos::SharedLock lock(this->mutex);
      return this->EOLpattern;
    }
    
    void setEOLPattern(boost::regex EOLPattern)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->EOLpattern = EOLPattern;
    }
    
    void setQuestion(std::string question)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->question = question;
    }
    
    void receive(std::string answer)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->busyLock.reset();
      if(this->promise)
      {
        this->promise->set_value(answer);
        debugLog(std::string("Query received: ") + answer);
        return;
      }
      debugLog(std::string("Query ignored: ") + answer);
      this->done = true;
    }
    
    void setError(std::string errorMessage)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->busyLock.reset();
      this->error = true;
      try
      {
	this->promise->set_exception(make_exception_ptr(exception() << errorName("query_got_error") << errorSeverity(severity::Informative) << errorDescription(errorMessage) ));
	this->lastErrorMessage = errorMessage;
	debugLog(std::string("Query got error: ") + errorMessage);
      }
      catch(boost::promise_already_satisfied& e)
      {
	debugLog(std::string("Query promise already satisfied and new error:") + errorMessage);
      }
      this->done = true;
    }
    
    bool isSent()
    {
      lughos::SharedLock lock(this->mutex);
      return this->sent;
    }

    std::string spyAnswer()
    {
      lughos::SharedLock lock(this->mutex);
      this->answer->timed_wait(boost::posix_time::seconds(2));
      if(answer->has_value() || answer->has_exception())
        return this->answer->get();
      else
        BOOST_THROW_EXCEPTION(exception() << errorName("query_took_to_long") << errorSeverity(severity::Informative));
    }
    
    boost::asio::streambuf& input()
    {
      lughos::SharedLock lock(this->mutex);
      BOOST_ASSERT_MSG(busyLock,"NOT allowed to call input on query without busy-mode");
      return *this->response;
    }
    
    boost::asio::streambuf& output()
    {
      lughos::SharedLock lock(this->mutex);
      BOOST_ASSERT_MSG(busyLock,"NOT allowed to call output on query without busy-mode");
      std::ostream ostream ( request.get() );
      ostream << this->question;
      return *this->request;
    }
    
    void ready()
    {
      std::stringstream sstream;
      lughos::SharedLock lock(this->mutex);
      sstream << response.get() ;
      lock.unlock();
      this->receive(sstream.str());
    }
    
    void busy(boost::mutex& busyMutex)
    {
      this->busyLock.reset(new boost::unique_lock<boost::mutex>(busyMutex));
    }
    
    std::string getAnswer()
    {
      return this->spyAnswer();
    }
    
    void purge()
    {
    lughos::ExclusiveLock lock(this->mutex);
      this->promise.reset(new boost::promise<std::string>());
      this->answer.reset(new boost::shared_future<std::string>(this->promise->get_future()));
      this->request.reset(new boost::asio::streambuf());
      this->response.reset(new boost::asio::streambuf());
      this->question.clear();
      this->sent = false;
      this->done = false;
      this->error = false;
      this->lastErrorMessage.clear();
    }
    
    std::string getQuestion()
    {
      lughos::SharedLock lock(this->mutex);
      return this->question;
    }
    
//     std::string send(boost::mutex& busyMutex)
//     {
//       lughos::ExclusiveLock lock(this->mutex);
//       this->busyLock.reset(new boost::unique_lock<boost::mutex>(busyMutex));
//       this->promise.reset(new boost::promise<std::string>());
//       this->answer.reset(new boost::shared_future<std::string>(this->promise->get_future()));
//       this->sent = true;
//       debugLog(std::string("Query sent: ") + this->question);
//       return this->question;
//     }
  };

  class Query : public QueryImpl
  {
  public:
  
    Query ( std::string question ) : QueryImpl(question)
    {
        
    }
  
  };

} // namespace lughos
#endif