#ifndef EXPOSED_MEASURED_VALUES_HPP
#define EXPOSED_MEASURED_VALUES_HPP

#include "exposedValues.hpp"
#include "measuredValue.hpp"

namespace lughos
{
   template <class T> class exposedMeasurement : public ExposedValue<T>, public measuredValue<T>
   {
   private:
     Mutex mutex;
   protected:
     bool markedExpired_;
     boost::posix_time::time_duration timeToLive_;
     
   public:
    exposedMeasurement(std::string name) : measuredValue<T>() , ExposedValue<T>(name)
    {
      markedAsExpired_ = true;
      timeToLive_ = boost::posix_time::time_duration::seconds(1);
      this->onValueChange.connect(boost::bind(&exposedMeasurement<T>::markedExpired,this,false));
      this->onRequestValue;
    }
    
    bool hasExpired()
    {
      if(markedExpired_)
	return true;
      boost::posix_time::time_duration diff = boost::posix_time::microsec_clock::local_time() - this->timeStamp;
      if (diff > timeToLive_)
	return true;
      else
	return false;
    }
    
    void timeToLive(boost::posix_time::time_duration duration)
    {
      this->timeToLive_ = duration;
    }
    
    boost::posix_time::time_duration timeToLive()
    {
      return this->timeToLive_;
    }
    
    void markedExpired(bool hasExpired)
    {
      ExclusiveLock lock(mutex);
      this->markedAsExpired_ = hasExpired;
    }
    
    bool markedExpired() const
    {
      SharedLock lock(mutex);
      return this->markedAsExpired_;
    }
     
   };
} //namespace lughos

#endif