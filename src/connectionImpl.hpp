#ifndef CONNECTION_IMPL_HPP
#define CONNECTION_IMPL_HPP
#include <boost/smart_ptr/shared_ptr.hpp>
#include "threadSafety.hpp"
#include <boost/chrono.hpp>
#include <boost/regex.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/signals2/signal.hpp>

class QueryImpl
{
protected:
  bool sent;
  unsigned long int lastReadAnswer;
  bool awaitingAnswer;
  bool continous; //TODO add 
  bool done;
  
  boost::signals2::signal<void (std::vector<std::string>&)> onReceived;
  boost::signals2::signal<void (void)> onSent;
  
  std::string question;
  std::vector<std::string> answers;
  
  boost::shared_mutex mutex;
  boost::mutex answerWaitingMutex;
  
  boost::regex EOLpattern;
  
  
  
public:
  
  QueryImpl()
  {
    lughos::ExclusiveLock lock(this->mutex);
    this->answerWaitingMutex.lock();
    this->lastReadAnswer = 0;
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
    lughos::SharedLock lock(this->mutex);
    return this->answers.back();
  }
  
  std::string spyAnswer(unsigned long int number)
  {
    lughos::SharedLock lock(this->mutex);
    return this->answers[number];
  }
  
  std::string getAnswer()
  {
    this->answerWaitingMutex.lock();
    return this->spyAnswer();
  }
  
  std::string getAnswer(unsigned long int number)
  {
    this->answerWaitingMutex.lock();
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
  }
};

class Query : public QueryImpl
{};


/**
 * @class ConnectionImpl
 * @brief class describing an abstract connection
 * 
 */
class ConnectionImpl
{
public:
  
  virtual void setIoService(boost::shared_ptr< boost::asio::io_service > io_service)
  {
    this->io_service_ = io_service;
    this->timeoutTimer.reset(new boost::asio::deadline_timer(*io_service));
    start();
  }

  boost::shared_ptr< boost::asio::io_service > getIoService()
  {
    return this->io_service_;
  }
        
  /**
   * @brief trys to establish connection
   * 
   * @return bool
   */
  virtual bool testconnection()=0;
  /**
   * @brief sets a port for the connection
   * 
   * @param port port name as string
   * @return void
   */
  virtual void set_port(std::string port)=0;
  /**
   * @brief sets hardware bits
   * 
   * @return void
   */
  virtual void reset()=0;
  /**
   * @brief aborts the connection
   * 
   * @return void
   */
  virtual void abort() = 0;
  /**
   * @brief sends query, waits for resonse
   * 
   * @param query string with pure command without end of line caracter
   * @return int returns 1 if succeeded
   */
  virtual int  execute(boost::shared_ptr<Query> query, boost::regex regExpr = boost::regex())=0;
  /**
   * @brief sends query, does not wait for resonse
   * 
   * @param query string with pure command without end of line caracter
   * @return int returns 1 if succeeded
   */

protected:
  /**
   * @brief establishes the conntection
   * 
   * @return bool true if succeeded
   */
  virtual bool start()=0;
  /**
   * @brief stops the connection
   * 
   * @return void
   */
  virtual void stop()=0;
	boost::shared_ptr<Query> currentQuery;
	boost::shared_ptr< boost::asio::io_service > io_service_;
	boost::shared_ptr<boost::asio::deadline_timer> timeoutTimer;

// 	virtual int write(const std::string &buf)=0;
// 	virtual int write_async(const std::string &buf)=0;


	

};

template <class C> class ConnectionTemplate : public ConnectionImpl
{
public:
  C context;
};

template <class C> class Connection : public ConnectionTemplate<C>
{};
#endif
