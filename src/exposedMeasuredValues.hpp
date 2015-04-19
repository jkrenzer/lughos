#ifndef EXPOSED_MEASURED_VALUES_HPP
#define EXPOSED_MEASURED_VALUES_HPP

#include "exposedValues.hpp"
#include "measuredValue.hpp"
#include <boost/signals2/shared_connection_block.hpp>

namespace lughos
{
   template <class T> class exposedMeasurement : public ExposedValue<T>, public measuredValue<T>
   {
   private:
     Mutex mutex;
   protected:
     bool markedExpired_;
     boost::posix_time::time_duration timeToLive_;
     boost::function<T()> getter_;
     boost::function<void(T)> setter_;
     boost::signals2::connection syncConnection;
     
   public:
   
   
    exposedMeasurement(std::string name) : measuredValue<T>() , ExposedValue<T>(name)
    {
      markedExpired_ = true;
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
    
    void refresh()
    {
      if(getter_)
      {
	ExclusiveLock lock(mutex);
	*(this->valuePointer) = getter_();
      }
    }
    
    void sync()
    {
      if(setter_)
      {
	SharedLock lock(mutex);
	this->setter_(*(this->valuePointer));
      }
    }
    
    bool hasExpired()
    {
      SharedLock lock(mutex);
      if(markedExpired_)
	return true;
      boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - this->timeStamp;
      if (diff > timeToLive_)
	return true;
      else
	return false;
    }
    
    void getter(boost::function<T()> getter_)
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