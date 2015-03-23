#ifndef MEASURED_VALUE_HPP
#define MEASURED_VALUE_HPP
#include "makros.hpp"
#include "unitValue.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace lughos
{
  template <class T>
  class measuredValue : public unitValue<T>
  {
  protected:
    boost::posix_time::ptime timeStamp;
    std::string sensorName;
  public:
      measuredValue(T value, std::string unit, boost::posix_time::ptime timestamp = boost::posix_time::second_clock::local_time(), std::string sensorName ="unknown");
      
      measuredValue(void);
      
      virtual ~measuredValue();
      
//       measuredValue& operator=(measuredValue other);
//       
//       measuredValue& operator=(unitValue other);
      
      void setTimestamp(boost::posix_time::ptime timeStamp);
      
      boost::posix_time::ptime getTimestamp();
      
      void setSensorName(std::string sensorName);
      
      std::string getsensorName();
      
  };
  
  //Definition of template class
  
  template <class T>
  void measuredValue<T>::setTimestamp(boost::posix_time::ptime timeStamp)
  {
    this->timeStamp = timeStamp;
  }
  
  template <class T>
  boost::posix_time::ptime measuredValue<T>::getTimestamp()
  {
    return this->timeStamp;
  }
  
  template <class T>
  void measuredValue<T>::setSensorName(std::string sensorName)
  {
    this->sensorName = sensorName;
  }
  
  template <class T>
  std::string measuredValue<T>::getsensorName()
  {
    return this->sensorName;
  }
  
  template <class T>
  measuredValue<T>::measuredValue(T value, std::string unit, boost::posix_time::ptime timestamp, std::string sensorName): timeStamp(timestamp), sensorName(sensorName)
  {
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 support in float required");
    this->value = value;
    this->unit = unit;
  }
  
  template <class T>
  measuredValue<T>::measuredValue(void): unitValue<T>()
  {
    
  }
  
  template <class T>
  measuredValue<T>::~measuredValue(void)
  {
    
  }
  
//   template <class T>
//   measuredValue& measuredValue<T>::operator=(measuredValue other)
//   {
//     this->value = other.getvalue();
//     this->unit = other.getunit();
//     this->timestamp = other.gettimestamp();
//     this->sensorName = other.getsensorName();
//     return *this;
//   }
// 
//   template <class T>
//   measuredValue& measuredValue<T>::operator=(unitValue other)
//   {
//     this->value = other.getvalue();
//     this->unit = other.getunit();
//     return *this;
//   }
  
}
#endif

