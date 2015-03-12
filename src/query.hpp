#ifndef QUERY_HPP
#define QUERY_HPP
#include "threadSafety.hpp"
#include <boost/chrono.hpp>
#include <boost/regex.hpp>
#include <boost/signals2/signal.hpp>


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
    std::vector<std::string> answers;
    
    boost::shared_mutex mutex;
    boost::timed_mutex answerWaitingMutex;
    
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
      this->answerWaitingMutex.unlock();
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
      this->answers.push_back(answer);
      lock.unlock();
      this->answerWaitingMutex.unlock();
    }
    
    void setError(std::string errorMessage)
    {
      lughos::ExclusiveLock lock(this->mutex);
      this->error = true;
      this->lastErrorMessage = errorMessage;
      this->answerWaitingMutex.unlock();
    }
    
    bool isSent()
    {
      lughos::SharedLock lock(this->mutex);
      return this->sent;
    }
    
    bool hasNewAnswer()
    {
      lughos::SharedLock lock(this->mutex);
      return (this->answers.size() > this->lastReadAnswer);
    }
    
    std::string spyAnswer()
    {
      return this->spyAnswer(0);
    }
    
    std::string spyAnswer(unsigned long int number)
    {
      lughos::SharedLock lock(this->mutex);
      if(number < answers.size())
        return this->answers[number];
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
    
    std::string awaitNewAnswer(boost::posix_time::time_duration duration)
    {
      if (!hasNewAnswer())
        this->answerWaitingMutex.timed_lock(boost::get_system_time() + boost::posix_time::seconds(3));
        this->answerWaitingMutex.lock();
        this->answerWaitingMutex.unlock();
      return this->spyAnswer();
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
    }
  };

  class Query : public QueryImpl
  {};

} // namespace lughos
#endif