#ifndef MEASURED_VALUE_HPP
#define MEASURED_VALUE_HPP
#include "makros.hpp"
#include "unitValue.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace lughos
{
  class measuredValue : public unitValue
  {
  protected:
    boost::posix_time::ptime timestamp;
    std::string sensorName;
  public:
      measuredValue(double value, std::string unit, boost::posix_time::ptime timestamp = boost::posix_time::second_clock::local_time(), std::string sensorName ="unknown");
      
      measuredValue(void);
      
      DECLARE_GETTER_SETTER(timestamp,boost::posix_time::ptime)
      
      DECLARE_GETTER_SETTER(sensorName,std::string)
      
  };
}
#endif

