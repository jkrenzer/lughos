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
  private:
        mutable Mutex mutex;
  protected:
    
    bool sentBit;
    bool awaitingAnswer;
    bool continous;
    bool doneBit;
    bool error;
    static const unsigned int mayRetries = 3;
    int retryCounter;
    
//     boost::signals2::signal<void (std::vector<std::string>&)> onReceived;
//     boost::signals2::signal<void (void)> onSent;
    
    std::string question;
    boost::shared_ptr<boost::shared_future<std::string> > answer;
    boost::shared_ptr< boost::promise< std::string > > promise;
    boost::shared_ptr<boost::asio::streambuf> response;
    boost::shared_ptr<boost::asio::streambuf> request;
    
    bool busyBit;
    
    boost::regex EndOfReadingPattern;
    std::string EndOfSendingPattern;
    std::string lastErrorMessage;
    
    boost::posix_time::time_duration timeToLive_;
    
    
  public:

    boost::signals2::signal<void (void)> onCancel;
  
    const boost::uuids::uuid id = boost::uuids::random_generator()();
    
    const std::string idString = boost::lexical_cast<std::string>(id);
    
    QueryImpl(std::string question) : timeToLive_(boost::posix_time::seconds(1))
    {
      this->reset();
      this->setQuestion(question);
      lughos::ExclusiveLock lock(this->mutex);
      this->EndOfSendingPattern = std::string("\n");
      this->EndOfReadingPattern = boost::regex("\n");
    }
    
    ~QueryImpl()
    {
      this->reset();
    }
    
    void retry()
    {
      lughos::ExclusiveLock lock(this->mutex);
      if(this->retryCounter >= this->mayRetries)
      {
	this->setError(std::string("The query was aborted as the maximum retries were reached."));
	BOOST_THROW_EXCEPTION(lughos::exception() << errorName("max_retries_reached_query_cancel") << errorDescription("The query was aborted as the maximum retries were reached.") << errorSeverity(severity::Informative));
      }
      ++this->retryCounter;
    }
    
    boost::regex getEORPattern()
    {
      lughos::SharedLock lock(this->mutex);
      return this->EndOfReadingPattern;
    }
    
    boost::posix_time::time_duration timeToLive()
    {
      lughos::SharedLock lock(this->mutex);
      return this->timeToLive_;
    }
    
    void timeToLive(boost::posix_time::time_duration timeToLive_)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->timeToLive_ = timeToLive_;
    }
    
    void setEORPattern(boost::regex EOLPattern)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->EndOfReadingPattern = EOLPattern;
    }
    
    std::string getEOSPattern()
    {
      lughos::SharedLock lock(this->mutex);
      return this->EndOfSendingPattern;
    }
    
    void setEORPattern(std::string EOSPattern)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->EndOfSendingPattern = EOSPattern;
    }
    
    void setQuestion(std::string question)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->question = question;
    }
    
    void receive(std::string answer)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->busyBit = false;
      if(this->promise && !this->answer->has_value() && !this->answer->has_exception())
      {
        this->promise->set_value(answer);
        LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Query ")+ idString + std::string(" received: ") + answer) ;
        return;
      }
      LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Query ")+ idString + std::string(" ignored: ") + answer) ;
      this->doneBit = true;
    }
    
    void setError(std::string errorMessage)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->busyBit = false;
      this->error = true;
      try //TODO why don't we end up here on connection-timeout?
      {
	this->promise->set_exception(make_exception_ptr(exception() << errorName("query_got_error") << errorSeverity(severity::Informative) << errorDescription(errorMessage) ));
	this->lastErrorMessage = errorMessage;
	LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Query ")+ idString + std::string(" got error: ") + errorMessage) ;
      }
      catch(boost::promise_already_satisfied& e)
      {
	LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Query ")+ idString + std::string(" promise already satisfied and new error:") + errorMessage) ;
      }
      this->doneBit = true;
    }
    
    bool isSent()
    {
      lughos::SharedLock lock(this->mutex);
      return this->sentBit;
    }

    std::string spyAnswer()
    {
      lughos::SharedLock lock(this->mutex);
      if(!answer->has_value() && !answer->has_exception())
      {
        LUGHOS_LOG(log::SeverityLevel::informative) << "Query " << idString << " with question " << this->question << " is waiting for answer..." ;
        lock.unlock();
        this->answer->timed_wait(timeToLive_); //Never wait locked!
        lock.lock();
      }
      if(answer->has_value())
      {
        std::string tmp = this->answer->get();
        LUGHOS_LOG(log::SeverityLevel::informative) << "Query " << idString << " with question " << this->question << " has answer: " << tmp ;
        return tmp;
      }
      else if (answer->has_exception())
      {
        LUGHOS_LOG(log::SeverityLevel::informative) << "Query " << idString << " with question " << this->question << " got exception. Rethrowing. " ;
        this->answer->get();
      }
      else
      {
	LUGHOS_LOG(log::SeverityLevel::informative) << "Query " << idString << " with question " << this->question << " timed out on client side!" ;
	BOOST_THROW_EXCEPTION( exception() << errorName("query_answer_timed_out") << errorDescription(idString + std::string(" with question ") + question + std::string(" timed out.")) << errorSeverity(severity::MustNot) );
	}
    }
    
    boost::asio::streambuf& input()
    {
      lughos::SharedLock lock(this->mutex);
      if(busyBit)
	return *this->response;
      else
      {
	LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Query ")+ idString + std::string(": Not in busy mode! Call disallowed.")) ;
	BOOST_THROW_EXCEPTION( exception() << errorName("call_not_allowed") << errorDescription(idString + std::string(": Not in busy mode! Call disallowed.")) << errorSeverity(severity::MustNot) );
      }
    }
    
    boost::asio::streambuf& output()
    {
      lughos::SharedLock lock(this->mutex);
//       BOOST_ASSERT_MSG(busyLock,idString.c_str());
      std::ostream ostream ( request.get() );
      ostream << this->question << this->EndOfSendingPattern;
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
    
    void busy(bool busy)
    {
      ExclusiveLock lock(mutex);
      this->busyBit = busy;
    }
    
    bool busy() const
    {
      SharedLock lock(mutex);
      return this->busyBit;
    }
    
    std::string getAnswer()
    {
      return this->spyAnswer();
    }
    
    void cancel()
    {
      this->onCancel();
      this->busyBit = false;
      this->sentBit = false;
      this->doneBit = false;
      this->error = false;
    }
    
    void reset()
    {
//       if (promise)
//       try 
//       {
//         this->promise->set_exception(make_exception_ptr(exception() << errorName("query_reset_abort") << errorSeverity(severity::Informative) << errorDescription("Query was reset so the waiting operations are aborted.") ));
//         this->answer->timed_wait(boost::posix_time::seconds(3));
//       }
//       catch(...)
//       {}

      lughos::ExclusiveLock lock(this->mutex);
      this->answer.reset();
      this->answer.reset(new boost::shared_future<std::string>());
      this->promise.reset(new boost::promise<std::string>());
      *this->answer = this->promise->get_future();
      this->request.reset(new boost::asio::streambuf());
      this->response.reset(new boost::asio::streambuf());
      this->cancel();
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
//       LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Query sent: ") + this->question) ;
//       return this->question;
//     }
  };

  class Query : public QueryImpl
  {
  public:
    
    Query (const char* questionCStr) : QueryImpl(std::string(questionCStr))
    {
    }
  
    Query ( std::string question ) : QueryImpl(question)
    {
        
    }
  
  };

} // namespace lughos
#endif