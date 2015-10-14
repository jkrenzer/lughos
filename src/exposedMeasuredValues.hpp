#ifndef EXPOSED_MEASURED_VALUES_HPP
#define EXPOSED_MEASURED_VALUES_HPP

#include "exposedValues.hpp"
#include "measuredValue.hpp"
#include <boost/signals2/shared_connection_block.hpp>
#include <boost/optional/optional.hpp>
#include <boost/asio/io_service.hpp>

namespace lughos
{
   template <class T> class exposedMeasurement : public ExposedValue<T>, public measuredValue<T>
   {
   private:
     Mutex mutex;
   protected:
     bool markedExpired_;
     bool expires_;
     bool alwaysRefresh_;
     bool isInitialPull_;
     boost::posix_time::time_duration timeToLive_;
     boost::function<void(measuredValue<T>&)> refresher_;
     boost::function<measuredValue<T>()> getter_;
     boost::function<void(measuredValue<T>&)> setter_;
     boost::signals2::connection syncConnection;
     boost::shared_ptr<boost::asio::io_service> ioService;
     
    void refresh_()
    {
      LUGHOS_LOG_FUNCTION();
      LUGHOS_LOG(log::SeverityLevel::debug) << "Starting refresh of " << this->name;
      UpgradeLock lock(mutex);
      if (refresher_)
      {
	LUGHOS_LOG(log::SeverityLevel::debug) << (std::string("Firing refresh-function for ") + this->name);
	try
	{
	  this->isInitialPull_ = false;
	  lock.unlock();
	  refresher_(dynamic_cast<measuredValue<T> &>(*this));
	  return;
	}
	catch(exception& e)
	{
	  LUGHOS_LOG(log::SeverityLevel::error) << (std::string("Exception thrown by refresh-function for ") + this->name) << ". What: " << e.what();
	}
	catch(...)
	{
	  LUGHOS_LOG(log::SeverityLevel::error) << (std::string("Unknown exception thrown by refresh-function for ") + this->name);
	}
      }
      else if(getter_)
      {
        measuredValue<T> newValue;
	LUGHOS_LOG(log::SeverityLevel::debug) << (std::string("Firing getter-function for ") + this->name) ;
	try 
	{
	  newValue = getter_();
	}
	catch(exception& e)
	{
	  LUGHOS_LOG(log::SeverityLevel::error) << (std::string("Exception thrown by getter-function for ") + this->name) << ". What: " << e.what();
	}
	catch(...)
	{
	  LUGHOS_LOG(log::SeverityLevel::error) << (std::string("Unknown exception thrown by getter-function for ") + this->name);
	}
	if(newValue.isSet())
	{
	  T tmp = *(this->valuePointer);
	  lock.unlock();
	  *( dynamic_cast<measuredValue<T>* >(this)) = newValue;
	  lock.lock();
	  
	  if(tmp != *(this->valuePointer))
	  {
	    lock.unlock();
	    this->onValueChange();
	    LUGHOS_LOG(log::SeverityLevel::debug) << "Fetched new value \"" << this->type.toString(newValue) << "\" for " << this->name << ". Old value was: " << this->type.toString(tmp);
	    upgradeLockToExclusive llock(lock);
	    this->isInitialPull_ = false;
	    return;
	  }
	  else
	    LUGHOS_LOG(log::SeverityLevel::debug) << (std::string("No new value availible for ") + this->name) ;
	}
	else
	  LUGHOS_LOG(log::SeverityLevel::error) << "Did not get a new value to compare with old value for " << this->name;
      }
      else
	LUGHOS_LOG(log::SeverityLevel::error) << "No getter or refresher for " << this->name << " set!"  ;
    }
    
      
    
    void sync_()
    {
      LUGHOS_LOG_FUNCTION();
      this->expires(false);
      SharedLock lock(mutex);
      if(setter_)
      {
	this->setter_(*( dynamic_cast<measuredValue<T>* >(this)));
	LUGHOS_LOG(log::SeverityLevel::informative) << "Firing setter for \"" << this->name << "\"." ;
      }
      lock.unlock();
      this->expires(true);
    }
     
   public:
   
    exposedMeasurement<T>(exposedMeasurement<T>& other) : measuredValue<T>(other), ExposedValue<T>(other)
    {
      ExclusiveLock lock(mutex);
      alwaysRefresh_ = false;
      markedExpired_ = other.markedExpired_;
      expires_ = other.expires_;
      timeToLive_ = other.timeToLive_;
      isInitialPull_ = other.isInitialPull_;
      this->getter_ = other.getter_;
      this->setter_ = other.setter_;
      this->ioService = other.ioService;
      this->onValueChange.connect(boost::bind(&exposedMeasurement<T>::markedExpired,this,false));
      this->syncConnection = this->onValueChange.connect(boost::bind(&exposedMeasurement<T>::sync,this));
      this->beforeReadValue.connect(boost::bind(&exposedMeasurement<T>::refreshIfExpired,this));
    }
   
    exposedMeasurement(std::string name, boost::shared_ptr<boost::asio::io_service>& ioService) : measuredValue<T>() , ExposedValue<T>(name), ioService(ioService)
    {
      ExclusiveLock lock(mutex);
      alwaysRefresh_ = false;
      markedExpired_ = true;
      expires_ = true;
      isInitialPull_ = true;
      timeToLive_ = boost::posix_time::seconds(1);
      this->onValueChange.connect(boost::bind(&exposedMeasurement<T>::markedExpired,this,false));
      this->syncConnection = this->onValueChange.connect(boost::bind(&exposedMeasurement<T>::sync,this));
      this->beforeReadValue.connect(boost::bind(&exposedMeasurement<T>::refreshIfExpired,this));
    }
    
    ~exposedMeasurement()
    {
      ExclusiveLock lock(mutex);
    }
    
    void refreshIfExpired()
    {
      if(hasExpired() || isInitialPull_)
	this->refresh();
    }
    
    void refresh()
    {
      LUGHOS_LOG_FUNCTION();
      SharedLock lock(mutex);
      if(this->ioService)
      {
        LUGHOS_LOG(log::SeverityLevel::debug) << "Refreshing " << this->name << " asynchronously.";
	this->ioService->post(boost::bind(&exposedMeasurement::refresh_,this));
      }
      else
      {
        LUGHOS_LOG(log::SeverityLevel::debug) << "Refreshing " << this->name << " synchronously.";
        lock.unlock();
	this->refresh_();
      }
    }
    
    void sync()
    {
      LUGHOS_LOG_FUNCTION();
      SharedLock lock(mutex);
      if(this->ioService)
      {
        LUGHOS_LOG(log::SeverityLevel::debug) << "Syncing " << this->name << " asynchronously.";
	this->ioService->post(boost::bind(&exposedMeasurement::sync_,this));
      }
      else
      {
        LUGHOS_LOG(log::SeverityLevel::debug) << "Syncing " << this->name << " synchronously.";
        lock.unlock();
	this->sync_();
      }
    }
    
    measuredValue<T>& get()
    {
      this->refreshIfExpired();
      SharedLock lock(mutex);
      return dynamic_cast<measuredValue<T> &>(*this);
    }
    
    bool hasExpired()
    {
      SharedLock lock(mutex);
      if(markedExpired_ || alwaysRefresh_ || isInitialPull_)
	return true;
      if(expires_)
      {
      boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - this->timeStamp;
      if (diff > timeToLive_)
	return true;
      }
      return false;
    }
    
    void expires(bool expires_ = true, bool always_ = false)
    {
      ExclusiveLock lock(mutex);
      this->expires_ = expires_;
      this->alwaysRefresh_ = always_;
    }
    
    bool expires()
    {
      SharedLock lock(mutex);
      return this->expires_;
    }
    
    void refresher(boost::function<void(measuredValue<T>&)> refresher_)
    {
      ExclusiveLock lock(mutex);
      this->refresher_ = refresher_;
    }
    
    void getter(boost::function<measuredValue<T>()> getter_)
    {
      ExclusiveLock lock(mutex);
      this->getter_ = getter_;
    }
    
    boost::function<T()> getter()
    {
      SharedLock lock(mutex);
      return this->getter_;
    }
    
    void setter(boost::function<void(T)> setter_)
    {
      ExclusiveLock lock(mutex);
      this->setter_ = setter_;
    }
    
    boost::function<void(T)> setter()
    {
      SharedLock lock(mutex);
      return this->setter_;
    }
    
    void timeToLive(boost::posix_time::time_duration duration)
    {
      ExclusiveLock lock(mutex);
      this->timeToLive_ = duration;
    }
    
    boost::posix_time::time_duration timeToLive()
    {
      SharedLock lock(mutex);
      return this->timeToLive_;
    }
    
    void markedExpired(bool hasExpired)
    {
      ExclusiveLock lock(mutex);
      this->markedExpired_ = hasExpired;
    }
    
    bool markedExpired() const
    {
      SharedLock lock(mutex);
      return this->markedExpired_;
    }
     
   };
} //namespace lughos

#endif