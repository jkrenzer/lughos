#ifndef TEST_GUI_MONITORING_HPP
#define TEST_GUI_MONITORING_HPP
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include "MaxiGauge.hpp"
#include "kithleighSerial.hpp"
#include "measuredValue.hpp"
#include "jobQueue.hpp"
#include "measuredDBValue.hpp"
using namespace lughos;

class PressureMeasurement : public measuredValue
{
  
};

class KeithleyTest : public Task
{
  protected:
  
    boost::shared_ptr<kithleighSerial> keithley;
    boost::shared_ptr<dbo::Session> session;
  
    virtual void run()
    { 
     if( keithley->isConnected())
      { 
	try{
	  measuredValue measure = this->keithley->getMeasure();

	  dbo::Transaction transaction(*session);
	  this->session->add(static_cast<measuredDBValue*>(new measuredValue(measure.getvalue(),measure.getunit(),measure.gettimestamp(), keithley->getName())));
	  transaction.commit();
	  std::cout << "[*] Keithley " << boost::posix_time::second_clock::local_time() << " -> " << measure.getvalue()<<" "<<measure.getunit()<< std::endl;
	}
	catch(...)
	{
	  
	}
     }
       
    }
    
        virtual void init()
    {
     if( keithley->isConnected())
      {
	try{
	this->keithley->input("*RST");
	  }
	catch(...)
	{
	  
	}
     }
    }
public:
  
    KeithleyTest(boost::shared_ptr<dbo::Session> session,boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> keithley) : session(session),Task(executionQueuePtr), keithley(boost::dynamic_pointer_cast<kithleighSerial>(keithley))
    {
    
    }
  
};


class PressureMonitor : public Task
{
  protected:
  
    boost::shared_ptr<MaxiGauge> maxiGauge;
    boost::shared_ptr<dbo::Session> session;
    int sensor;
  
    virtual void run()
    {
     if( maxiGauge->isConnected())
      { 
	try
	{
	  measuredValue measure = this->maxiGauge->getPressure(1);
	  dbo::Transaction transaction(*session);
	  this->session->add(static_cast<measuredDBValue*>(new measuredValue(measure.getvalue(),measure.getunit(),measure.gettimestamp(), maxiGauge->getName()+std::to_string(sensor))));
	  transaction.commit();
	  std::cout << "[*] MaxiGauge " << boost::posix_time::second_clock::local_time() << " -> " << measure.getvalue()<<" "<<measure.getunit()<< std::endl;
	}
	catch (...)
	{
	}
     }  
    }
    
        virtual void init()
    {
    }
public:
  
    PressureMonitor(boost::shared_ptr<dbo::Session> session,boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> maxiGauge, int sensor) : session(session),Task(executionQueuePtr), maxiGauge(boost::dynamic_pointer_cast<MaxiGauge>(maxiGauge))
    {
      this->sensor = sensor;    
    }
  
};

// class PressureMonitor : public Task
// {
// protected:
//   
//     boost::shared_ptr<MaxiGauge> maxiGauge;
//     boost::shared_ptr<dbo::Session> session;
//     int sensor;
//     virtual void run()
//     {
//       measuredValue pressure = this->maxiGauge->getPressure(sensor);
// //       dbo::Transaction transaction(*session);
// //       this->session->add(static_cast<measuredDBValue*>(new measuredValue(pressure.getvalue(),pressure.getunit(),pressure.gettimestamp(), maxiGauge->getName())));
// //       transaction.commit();
//       std::cout << "[*] MaxiGauge " << this->sensor << " @ " << boost::posix_time::second_clock::local_time() << " -> " << pressure.getvalue() << pressure.getunit() << std::endl;
//     }
//     
// public:
//   
//     PressureMonitor(boost::shared_ptr<dbo::Session> session,boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> maxiGauge, int sensor) : session(session),Task(executionQueuePtr), maxiGauge(boost::dynamic_pointer_cast<MaxiGauge>(maxiGauge))
//     {
//       this->sensor = sensor;
//     }
//   
// };
#endif