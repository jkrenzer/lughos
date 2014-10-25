#ifndef MEASURED_VALUE_CPP
#define MEASURED_VALUE_CPP
#include "measuredValue.hpp"

namespace lughos {

  DEFINE_GETTER_SETTER(measuredValue,timestamp,boost::posix_time::ptime)

  DEFINE_GETTER_SETTER(measuredValue,sensorName,std::string)

  measuredValue::measuredValue(double value, std::string unit, boost::posix_time::ptime timestamp): timestamp(timestamp)
  {
    static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 support in float required");
    this->value = value;
    this->unit = unit;
  }
  
  measuredValue::measuredValue(void): unitValue()
  {
    
  }
  


}


#endif