#ifndef DATA_ACQUISITION_HPP
#define DATA_ACQUISITION_HPP
#include "measuredDBValue.hpp"
#include "exposedMeasuredValues.hpp"
#include "log.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/placeholders.hpp>

namespace lughos 
{
  class DataAcquisition
  {
    private:
      Mutex mutex;
    protected:
      boost::shared_ptr<dbo::Session> databaseSession;
      boost::shared_ptr<boost::asio::io_service> ioService;
      boost::posix_time::time_duration timeBetweenMeasurements;
      boost::shared_ptr<boost::asio::deadline_timer> timer;
      boost::shared_ptr<boost::signals2::signal<void ()> > queue_;
      
      void loop_(const boost::system::error_code& error)
      {
	if(!error)
	{
	  debugLog(std::string("Acquiring data..."));
	  try
	  { (*this->queue_)();
	    debugLog(std::string("Acquired data sucessfully."));
	  }
	  catch(std::exception& e)
	  {
	    debugLog(std::string("Error in data acquisition. Message: ") + e.what());
	  }
	  this->start();
	}
	else
	  debugLog(std::string("Aborting acquisition loop. Cause: ") + error.message());
      }
      
      template <class T>
      void acquire_(exposedMeasurement<T>* value)
      {
	typedef Wt::Dbo::ptr<measuredDBValue<T> > valuePtr;
	value->refreshIfExpired();
	measuredDBValue<T>* ptr (new measuredDBValue<T>(*value));
	Wt::Dbo::Transaction transaction(*this->databaseSession);
	this->databaseSession->add(ptr);
	transaction.commit();
      }

    public:

      DataAcquisition(boost::shared_ptr<boost::asio::io_service> ioService, boost::shared_ptr<dbo::Session> databaseSession) : timeBetweenMeasurements(boost::posix_time::seconds(1)), ioService(ioService), databaseSession(databaseSession)
      {
	this->timer.reset(new boost::asio::deadline_timer(*ioService));
	this->queue_.reset(new boost::signals2::signal<void ()>());
      }
      
      DataAcquisition(DataAcquisition& other)
      {
	ExclusiveLock lock(other.mutex);
	this->databaseSession = other.databaseSession;
	this->timeBetweenMeasurements = other.timeBetweenMeasurements;
	this->timer = other.timer;
	this->queue_ = other.queue_;
	this->ioService = other.ioService;
      }

      template <class T>
      void acquire(exposedMeasurement<T>* value)
      {
	this->queue_->connect(boost::bind(&DataAcquisition::acquire_<T>,this,value));
      }
      
      void start()
      { 
	  this->timer->expires_from_now(this->timeBetweenMeasurements);
	  this->timer->async_wait(boost::bind(&DataAcquisition::loop_,this,boost::asio::placeholders::error()));
      }
      
      void stop()
      {
	this->timer->cancel();
      }
      
      void restart()
      {
	this->stop();
	this->start();
      }
    
  };
}//namespace lughos

#endif