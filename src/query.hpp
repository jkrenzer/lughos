#ifndef QUERY_HPP
#define QUERY_HPP
#include "threadSafety.hpp"
#include "log.hpp"
#include <boost/chrono.hpp>
#include <boost/regex.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread/future.hpp>
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
      if(this->promise)
      {
        this->promise->set_value(answer);
        this->promise.reset();
        debugLog(std::string("Query received: ") + answer);
        return;
      }
      debugLog(std::string("Query ignored: ") + answer);
       
    }
    
    void setError(std::string errorMessage)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->error = true;
      this->lastErrorMessage = errorMessage;
      debugLog(std::string("Query got error: ") + errorMessage);
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
      if(answer->has_value())
        return this->answer->get();
      else
        return std::string("");
    }
    
    boost::asio::streambuf& input()
    {
      return *this->response;
    }
    
    boost::asio::streambuf& output()
    {
      std::ostream ostream ( request.get() );
      ostream << this->question;
      return *this->request;
    }
    
    void ready()
    {
      std::stringstream sstream;
      sstream << response.get() ;
      this->receive(sstream.str());
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
      this->request.reset(new boost::asio::streambuf());
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
    
    std::string send(bool sent = true)
    {
      this->promise.reset(new boost::promise<std::string>());
      this->answer.reset(new boost::shared_future<std::string>(this->promise->get_future()));
      lughos::ExclusiveLock lock(this->mutex);
      this->sent = sent;
      debugLog(std::string("Query sent: ") + this->question);
      return this->question;
    }
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