#ifndef EXPOSED_MEASURED_VALUES_HPP
#define EXPOSED_MEASURED_VALUES_HPP

#include "exposedValues.hpp"
#include "measuredValue.hpp"
#include <boost/signals2/shared_connection_block.hpp>
#include <boost/optional/optional.hpp>

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
     boost::posix_time::time_duration timeToLive_;
     boost::function<void(measuredValue<T>&)> refresher_;
     boost::function<measuredValue<T>()> getter_;
     boost::function<void(measuredValue<T>&)> setter_;
     boost::signals2::connection syncConnection;
     
   public:
   
    exposedMeasurement<T>(exposedMeasurement<T>& other) : measuredValue<T>(other), ExposedValue<T>(other)
    {
      markedExpired_ = other.markedExpired_;
      expires_ = other.expires_;
      timeToLive_ = other.timeToLive_;
      this->getter_ = other.getter_;
      this->setter_ = other.setter_;
    }
   
    exposedMeasurement(std::string name) : measuredValue<T>() , ExposedValue<T>(name)
    {
      markedExpired_ = true;
      expires_ = true;
      timeToLive_ = boost::posix_time::seconds(1);
      this->onValueChange.connect(boost::bind(&exposedMeasurement<T>::markedExpired,this,false));
      this->syncConnection = this->onValueChange.connect(boost::bind(&exposedMeasurement<T>::sync,this));
      this->beforeReadValue.connect(boost::bind(&exposedMeasurement<T>::refreshIfExpired,this));
    }
    
    void refreshIfExpired()
    {
      if(hasExpired())
	this->refresh();
    }
    
    measuredValue<T>& get()
    {
      this->refreshIfExpired();
      return static_cast<measuredValue<T> &>(*this);
    }
    
    void refresh()
    {
      ExclusiveLock lock(mutex);
      if (refresher_)
      {
	LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Firing refresh-function for ") + this->name);
	try
	{
	  lock.unlock();
	  refresher_(static_cast<measuredValue<T> &>(*this));
	  lock.lock();
	  return;
	}
	catch(exception& e)
	{
	  LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Exception thrown by refresh-function for ") + this->name) << ". What: " << e.what();
	}
      }
      else if(getter_)
      {
	LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("Fetching new value for ") + this->name) ;
	measuredValue<T> newValue = getter_();
	if(newValue.isSet())
	{
	  T tmp = *(this->valuePointer);
	  lock.unlock();
	  *( dynamic_cast<measuredValue<T>* >(this)) = newValue;
	  lock.lock();
	  if(tmp != *(this->valuePointer))
	  {
	    lock.unlock();
	    std::stringstream ss;
	    ss << "New value \"" << Value<T>::getValueAsString() << "\" for " << this->name;
	    LUGHOS_LOG(log::SeverityLevel::informative) << (ss.str()) ;
	    this->onValueChange();
	  }
	  else
	    LUGHOS_LOG(log::SeverityLevel::informative) << (std::string("No new value given for ") + this->name) ;
	}
      }
    }
    
      
    
    void sync()
    {
      SharedLock lock(mutex);
      if(setter_)
      {
	lock.unlock();
	this->setter_(*( dynamic_cast<measuredValue<T>* >(this)));
      }
    }
    
    bool hasExpired()
    {
      SharedLock lock(mutex);
      if(markedExpired_ || alwaysRefresh_)
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
    
    void setter(boost::function<void(measuredValue<T>)> setter_)
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