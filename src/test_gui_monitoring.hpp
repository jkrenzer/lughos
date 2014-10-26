#ifndef TEST_GUI_MONITORING_HPP
#define TEST_GUI_MONITORING_HPP
#include "MaxiGauge.hpp"
#include "kithleighSerial.hpp"
#include "measuredValue.hpp"
#include "jobQueue.hpp"
#include <Wt/Dbo/Dbo>

using namespace lughos;

class PressureMeasurement : public measuredValue
{
  
};

class KeithleyTest : public Task
{
  protected:
  
    boost::shared_ptr<kithleighSerial> keithley;
  
    virtual void run()
    {
      std::cout << "[*] Keithley " << boost::posix_time::second_clock::local_time() << " -> " << this->keithley->inputOutput("READ?") << std::endl;
    }
    
        virtual void init()
    {
     this->keithley->inputOutput("*RST");
    }
public:
  
    KeithleyTest(boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> keithley) : Task(executionQueuePtr), keithley(boost::dynamic_pointer_cast<kithleighSerial>(keithley))
    {
    
    }
  
};

class PressureMonitor : public Task
{
protected:
  
    boost::shared_ptr<MaxiGauge> maxiGauge;
    int sensor;
  
    virtual void run()
    {
      measuredValue pressure = this->maxiGauge->getPressure(sensor);
      std::cout << "[*] MaxiGauge " << this->sensor << " @ " << boost::posix_time::second_clock::local_time() << " -> " << pressure.getvalue() << pressure.getunit() << std::endl;
    }
    
public:
  
    PressureMonitor(boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> maxiGauge, int sensor) : Task(executionQueuePtr), maxiGauge(boost::dynamic_pointer_cast<MaxiGauge>(maxiGauge))
    {
      this->sensor = sensor;
    }
  
};
#endif