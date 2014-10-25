#ifndef JOBQUEUE_HPP
#define JOBQUEUE_HPP

namespace lughos 
{

  class Task
  {
  public:
    
    enum State {uninitialized, ready, running, stopped, aborted, finished, error};
    
    enum Execute {infinite = -1};
    
  protected:
    Mutex mutex;
    int executeTimes;
    int executed;
    boost::posix_time::time_duration every;
    boost::posix_time::ptime beginExecution;
    boost::posix_time::ptime lastExecution;
    boost::posix_time::ptime endExecution;
    boost::shared_ptr<boost::asio::io_service> executionQueuePtr;
    boost::asio::deadline_timer timer;
    Task::State state;
    
    
    void exec()
    {
      GUARD
      this->run();
      this->executed++;
      this->lastExecution = boost::posix_time::microsec_clock::local_time();
      //TODO Add timed-end-and-begin-stuff
      if(executed < executeTimes || executeTimes <= -1)
	this->start();
    }
    
    
    
    virtual void run() = 0;
    
    
  public:
    
    Task(boost::shared_ptr<boost::asio::io_service> executionQueuePtr) : executionQueuePtr(executionQueuePtr), timer(*executionQueuePtr)
    {
      this->executed = 0;
      this->executeTimes = 0;
    }
    
    ~Task()
    {
    }
    
    virtual void init()
    {
    }
    
    virtual void shutdown()
    {
    }
    
    void setExecuteTimes(int times)
    {
      GUARD
      this->executeTimes = times;
    }
    
    int getExecuteTimes()
    {
      GUARD
      return this->executeTimes;
    }
    
    void setEvery(boost::posix_time::time_duration every)
    {
      GUARD
      this->every = every;
    }
    
    boost::posix_time::time_duration getEvery()
    {
      GUARD
      return this->every;
    }
    
    void start()
    {
      GUARD
      if(this->state ==Task::State::uninitialized)
	this->init();
      if(this->state ==Task::State::finished || this->state ==Task::State::aborted || this->state ==Task::State::error)
	return;
      if(!this->every.is_not_a_date_time() && (this->executeTimes > 0 || this->executeTimes <= -1 ))
      {
	if(this->executed != 0)
	{
	  this->timer.expires_from_now(every);
	  this->timer.async_wait(boost::bind(&Task::exec,this));
	}
	else
	{
	  this->executionQueuePtr->post(boost::bind(&Task::exec,this));
	  this->beginExecution = boost::posix_time::microsec_clock::local_time();
	}
      }
    }
    
    void stop()
    {
      if(this->state ==Task::State::running)
      {
	this->timer.cancel();
	this->state =Task::State::stopped;
      }
    }
    
  };

} //namespace lughos

#endif