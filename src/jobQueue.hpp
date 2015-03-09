#ifndef JOBQUEUE_HPP
#define JOBQUEUE_HPP
#include "threadSafety.hpp"
#include <boost/asio/deadline_timer.hpp>

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
      this->run();
      {
	ExclusiveLock lock(this->mutex);
	this->executed++;
	this->lastExecution = boost::posix_time::microsec_clock::local_time();
      }
      //TODO Add timed-end-and-begin-stuff
      if(this->executed < this->executeTimes || this->executeTimes <= -1)
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
      ExclusiveLock lock(this->mutex);
      this->executeTimes = times;
    }
    
    int getExecuteTimes()
    {
      SharedLock lock(this->mutex);
      return this->executeTimes;
    }
    
    void setEvery(boost::posix_time::time_duration every)
    {
      ExclusiveLock lock(this->mutex);
      this->every = every;
    }
    
    boost::posix_time::time_duration getEvery()
    {
      SharedLock lock(this->mutex);
      return this->every;
    }
    
    void start()
    {
      UpgradeLock lock(this->mutex);
      if(this->state ==Task::State::uninitialized)
      {
	lock.unlock();
	this->init();
	lock.lock();
      }
      if(this->state ==Task::State::finished || this->state ==Task::State::aborted || this->state ==Task::State::error)
	return;
      if(!this->every.is_not_a_date_time() && (this->executeTimes > 0 || this->executeTimes <= -1 ))
      {
	upgradeLockToExclusive llock(lock);
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
      UpgradeLock lock(this->mutex);
      if(this->state ==Task::State::running)
      {
	upgradeLockToExclusive llock(lock);
	this->timer.cancel();
	this->state =Task::State::stopped;
      }
    }
    
  };

} //namespace lughos

#endif