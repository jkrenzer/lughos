#ifndef MEASURED_VALUE_HPP
#define MEASURED_VALUE_HPP
#include "makros.hpp"
#include "unitValue.hpp"
#include "basicTypes.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace lughos
{
  class measuredValueInterface
  {
  protected:
    boost::posix_time::ptime timeStamp;
    int64_t timeStampTicks;
    std::string sensorName;
    
    int64_t timeStampToTicks(boost::posix_time::ptime timestamp)
    {
      return boost::posix_time::time_duration(timeStamp - boost::posix_time::ptime(boost::gregorian::date(1970,boost::gregorian::Jan,1))).total_milliseconds();
    }
    
  public:
    virtual void setTimeStamp(boost::posix_time::ptime timeStamp);
    virtual boost::posix_time::ptime getTimeStamp() const;
    virtual void setSensorName(std::string sensorName);
    virtual std::string getSensorName() const;
    
    measuredValueInterface(boost::posix_time::ptime timestamp = boost::posix_time::microsec_clock::local_time(), std::string sensorName = std::string("N/A")): timeStamp(timestamp), sensorName(sensorName)
    {
      this->timeStampTicks = timeStampToTicks(timestamp);
    }
    
  };
  
  template <class T>
  class measuredValue : public measuredValueInterface, public unitValueTemplate<T>
  {
  private:
    Mutex mutex;
  public:
      measuredValue(T value, std::string unit = std::string("") , boost::posix_time::ptime timestamp = boost::posix_time::microsec_clock::local_time(), std::string sensorName ="N/A");
      
      measuredValue(void);
      
      measuredValue(const measuredValue<T>& other);
      
      virtual ~measuredValue();
      
      measuredValue& operator=(measuredValue<T> other);
      
      measuredValue& operator=(unitValue<T> other);
      
      using Value<T>::operator=;    
  };
  
  //Definition of template class
  
  void measuredValueInterface::setTimeStamp(boost::posix_time::ptime timeStamp = boost::posix_time::microsec_clock::local_time())
  {
    this->timeStamp = timeStamp;
    this->timeStampTicks = timeStampToTicks(timeStamp);
  }
  
  boost::posix_time::ptime measuredValueInterface::getTimeStamp() const
  {
    return this->timeStamp;
  }
  
  void measuredValueInterface::setSensorName(std::string sensorName)
  {
    this->sensorName = sensorName;
  }
  
  std::string measuredValueInterface::getSensorName() const
  {
    return this->sensorName;
  }
  
  template <class T>
  measuredValue<T>::measuredValue(T value, std::string unit, boost::posix_time::ptime timestamp, std::string sensorName): measuredValueInterface(timestamp,sensorName), unitValueTemplate<T>(value,unit), mutex()
  {
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 support in float required");
    *this->valuePointer = value;
    this->unit = unit;
  }
  
  template <class T>
  measuredValue<T>::measuredValue(void): measuredValueInterface(), unitValueTemplate<T>(), mutex()
  {
    
  }
  
  template <class T>
  measuredValue<T>::~measuredValue(void)
  {
    
  }
  
  template <class T>
  measuredValue<T>& measuredValue<T>::operator=(measuredValue<T> other)
  {
    ExclusiveLock lock(this->mutex);
    *this->valuePointer = other.getValue();
    this->unit = other.getUnit();
    this->timeStamp = other.getTimeStamp();
    this->sensorName = other.getSensorName();
    return *this;
  }
  
  template <class T>
  measuredValue<T>::measuredValue(const measuredValue<T>& other) : mutex()
  {
    ExclusiveLock lock(this->mutex);
    *this->valuePointer = other.getValue();
    this->unit = other.getUnit();
    this->timeStamp = other.getTimeStamp();
    this->sensorName = other.getSensorName();
  }

  template <class T>
  measuredValue<T>& measuredValue<T>::operator=(unitValue<T> other)
  {
    ExclusiveLock lock(this->mutex);
    *this->valuePointer = other.getvalue();
    this->unit = other.getunit();
    return *this;
  }
  
}
#endif

