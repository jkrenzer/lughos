#ifndef MEASURED_VALUE_HPP
#define MEASURED_VALUE_HPP
#include "makros.hpp"
#include "unitValue.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace lughos
{
  class measuredValueInterface
  {
  protected:
    boost::posix_time::ptime timeStamp;
    std::string sensorName;
    
  public:
    virtual void setTimeStamp(boost::posix_time::ptime timeStamp);
    virtual boost::posix_time::ptime getTimeStamp();
    virtual void setSensorName(std::string sensorName);
    virtual std::string getSensorName();
    
    measuredValueInterface(boost::posix_time::ptime timestamp = boost::posix_time::microsec_clock::local_time(), std::string sensorName = std::string("N/A")): timeStamp(timestamp), sensorName(sensorName)
    {
      
    }
    
  };
  
  template <class T>
  class measuredValue : public measuredValueInterface, public unitValueTemplate<T>
  {

  public:
      measuredValue(T value, std::string unit, boost::posix_time::ptime timestamp = boost::posix_time::microsec_clock::local_time(), std::string sensorName ="N/A");
      
      measuredValue(void);
      
      virtual ~measuredValue();
      
      measuredValue& operator=(measuredValue<T> other);
      
      measuredValue& operator=(unitValue<T> other);
      
      using Value<T>::operator=;
    
  };
  
  //Definition of template class
  
  void measuredValueInterface::setTimeStamp(boost::posix_time::ptime timeStamp)
  {
    this->timeStamp = timeStamp;
  }
  
  boost::posix_time::ptime measuredValueInterface::getTimeStamp()
  {
    return this->timeStamp;
  }
  
  void measuredValueInterface::setSensorName(std::string sensorName)
  {
    this->sensorName = sensorName;
  }
  
  std::string measuredValueInterface::getSensorName()
  {
    return this->sensorName;
  }
  
  template <class T>
  measuredValue<T>::measuredValue(T value, std::string unit, boost::posix_time::ptime timestamp, std::string sensorName): measuredValueInterface(timestamp,sensorName)
  {
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 support in float required");
    *this->valuePointer = value;
    this->unit = unit;
  }
  
  template <class T>
  measuredValue<T>::measuredValue(void): unitValueTemplate<T>()
  {
    
  }
  
  template <class T>
  measuredValue<T>::~measuredValue(void)
  {
    
  }
  
  template <class T>
  measuredValue<T>& measuredValue<T>::operator=(measuredValue<T> other)
  {
    *this->valuePointer = other.getValue();
    this->unit = other.getUnit();
    this->timeStamp = other.getTimeStamp();
    this->sensorName = other.getSensorName();
    return *this;
  }

  template <class T>
  measuredValue<T>& measuredValue<T>::operator=(unitValue<T> other)
  {
    *this->valuePointer = other.getvalue();
    this->unit = other.getunit();
    return *this;
  }
  
}
#endif

