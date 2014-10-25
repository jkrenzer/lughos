#ifndef TEST_GUI_MONITORING_HPP
#define TEST_GUI_MONITORING_HPP
#include "MaxiGauge.hpp"
#include "measuredValue.hpp"
#include "jobQueue.hpp"
#include <Wt/Dbo/Dbo>

using namespace lughos;

class pressureMonitor : public Task
{
protected:
  
    MaxiGauge& maxiGauge;
    int sensor;
  
    virtual void run()
    {
      measuredValue pressure = this->maxiGauge.getPressure(sensor);
      std::cout << boost::posix_time::second_clock::local_time() << " -> " << pressure.getvalue() << pressure.getunit() << std::endl;
    }
    
public:
  
    pressureMonitor(boost::shared_ptr< boost::asio::io_service > executionQueuePtr, MaxiGauge& maxiGauge, int sensor) : Task(executionQueuePtr), maxiGauge(maxiGauge)
    {
      this->sensor = sensor;
    }
  
};
#endif