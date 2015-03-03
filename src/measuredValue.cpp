#ifndef MEASURED_VALUE_CPP
#define MEASURED_VALUE_CPP
#include "measuredValue.hpp"

namespace lughos {

  DEFINE_GETTER_SETTER(measuredValue,timestamp,boost::posix_time::ptime)

  DEFINE_GETTER_SETTER(measuredValue,sensorName,std::string)

  measuredValue::measuredValue(double value, std::string unit, boost::posix_time::ptime timestamp, std::string sensorName): timestamp(timestamp), sensorName(sensorName)
  {
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 support in float required");
    this->value = value;
    this->unit = unit;
  }
  
  measuredValue::measuredValue(void): unitValue()
  {
    
  }
  
measuredValue& measuredValue::operator=(measuredValue other)
{
  this->value = other.getvalue();
  this->unit = other.getunit();
  this->timestamp = other.gettimestamp();
  this->sensorName = other.getsensorName();
  return *this;
}

measuredValue& measuredValue::operator=(unitValue other)
{
  this->value = other.getvalue();
  this->unit = other.getunit();
  return *this;
}


}


#endif