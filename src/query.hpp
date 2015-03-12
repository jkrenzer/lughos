#ifndef QUERY_HPP
#define QUERY_HPP
#include "threadSafety.hpp"
#include <boost/chrono.hpp>
#include <boost/regex.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread/future.hpp>


namespace lughos
{
  

  class QueryImpl
  {
  protected:
    bool sent;
    unsigned long int lastReadAnswer;
    bool awaitingAnswer;
    bool continous; //TODO add 
    bool done;
    bool error;
    
    boost::signals2::signal<void (std::vector<std::string>&)> onReceived;
    boost::signals2::signal<void (void)> onSent;
    
    std::string question;
    std::vector<boost::shared_future<std::string> > answers;
    boost::shared_ptr< boost::promise< std::string > > promise;
    
    boost::shared_mutex mutex;
    
    boost::regex EOLpattern;
    
    std::string lastErrorMessage;
    
    
  public:
    
    QueryImpl()
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->lastReadAnswer = 0;
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
    
    void addAnswer(std::string answer)
    {
      lughos::ExclusiveLock lock(this->mutex);
      if(this->promise)
      {
        this->promise->set_value(answer);
        this->promise.reset();
      }
       
      lock.unlock();
    }
    
    void setError(std::string errorMessage)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->error = true;
      this->lastErrorMessage = errorMessage;
    }
    
    bool isSent()
    {
      lughos::SharedLock lock(this->mutex);
      return this->sent;
    }

    std::string spyAnswer()
    {
      lughos::SharedLock lock(this->mutex);
      return this->answers.back().get();
    }
    
    std::string spyAnswer(unsigned long int number)
    {
      lughos::SharedLock lock(this->mutex);
      if(number < answers.size())
        return this->answers[number].get();
      else
        return std::string("");
    }
    
    std::string getAnswer()
    {
      return this->spyAnswer();
    }
    
    std::string getAnswer(unsigned long int number)
    {
      return this->spyAnswer(number);
    }
    
    void purge()
    {
      this->answers.clear();
      this->lastReadAnswer = 0;
    }
    
    std::string getQuestion()
    {
      lughos::SharedLock lock(this->mutex);
      return this->question;
    }
    
    void setSent(bool sent = true)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->sent = sent;
      this->promise.reset(new boost::promise<std::string>());
      this->answers.push_back(promise->get_future());
    }
  };

  class Query : public QueryImpl
  {};

} // namespace lughos
#endif