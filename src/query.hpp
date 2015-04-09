#ifndef QUERY_HPP
#define QUERY_HPP
#include "threadSafety.hpp"
#include "errorHandling.hpp"
#include "log.hpp"
#include <boost/assert.hpp>
#include <boost/chrono.hpp>
#include <boost/regex.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
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
  
    const boost::uuids::uuid id = boost::uuids::random_generator()();
    
    const std::string idString = boost::lexical_cast<std::string>(id);
    
    QueryImpl(std::string question)
    {
      this->reset();
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
      if(this->promise && !this->answer->has_value() && !this->answer->has_exception())
      {
        this->promise->set_value(answer);
        debugLog(std::string("Query ")+ idString + std::string(" received: ") + answer);
        return;
      }
      debugLog(std::string("Query ")+ idString + std::string(" ignored: ") + answer);
      this->done = true;
    }
    
    void setError(std::string errorMessage)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->busyLock.reset();
      this->error = true;
      try //TODO why don't we end up here on connection-timeout?
      {
	this->promise->set_exception(make_exception_ptr(exception() << errorName("query_got_error") << errorSeverity(severity::Informative) << errorDescription(errorMessage) ));
	this->lastErrorMessage = errorMessage;
	debugLog(std::string("Query ")+ idString + std::string(" got error: ") + errorMessage);
      }
      catch(boost::promise_already_satisfied& e)
      {
	debugLog(std::string("Query ")+ idString + std::string(" promise already satisfied and new error:") + errorMessage);
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
//       if(this->busyLock->try_lock());
      this->answer->timed_wait(boost::posix_time::seconds(2));
      if(answer->has_value() || answer->has_exception())
      {
        std::string tmp = this->answer->get();
        debugLog(std::string("Query ")+ idString + std::string(" got answer: ") + tmp);
        return tmp;
      }
      else
      {
	debugLog(std::string("Query ")+ idString + std::string("timed out and has no answer."));
	BOOST_THROW_EXCEPTION( exception() << errorName("query_timed_out") << errorDescription(idString + std::string(" timed out.")) << errorSeverity(severity::MustNot) );
	}
    }
    
    boost::asio::streambuf& input()
    {
      lughos::SharedLock lock(this->mutex);
      if(busyLock)
	return *this->response;
      else
      {
	debugLog(std::string("Query ")+ idString + std::string(": Not in busy mode! Call disallowed."));
	BOOST_THROW_EXCEPTION( exception() << errorName("call_not_allowed") << errorDescription(idString + std::string(": Not in busy mode! Call disallowed.")) << errorSeverity(severity::MustNot) );
      }
    }
    
    boost::asio::streambuf& output()
    {
      lughos::SharedLock lock(this->mutex);
      BOOST_ASSERT_MSG(busyLock,idString.c_str());
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
    
    void reset()
    {
    lughos::ExclusiveLock lock(this->mutex);
      this->promise.reset(new boost::promise<std::string>());
      this->answer.reset(new boost::shared_future<std::string>(this->promise->get_future()));
      this->request.reset(new boost::asio::streambuf());
      this->response.reset(new boost::asio::streambuf());
      this->busyLock.reset();
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