/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

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
	  this->session->add(static_cast<measuredDBValue*>(new measuredValue(measure.getvalue(),measure.getunit(),measure.getTimeStamp(), keithley->getName())));
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
	  if(sensor!=0)
	  {  
	    measuredValue measure = this->maxiGauge->getPressure(sensor);
// 	    measuredValue measure= measuredValue(1.0,"a",boost::posix_time::second_clock::local_time(), "1" );
// 	    double value, std::string unit, boost::posix_time::ptime timestamp, std::string sensorName
	    dbo::Transaction transaction(*session);
	    this->session->add(static_cast<measuredDBValue*>(new measuredValue(measure.getvalue(),measure.getunit(),measure.getTimeStamp(), maxiGauge->getName()+std::to_string(sensor))));
	    transaction.commit();
	    std::cout << "[*] MaxiGauge " << boost::posix_time::second_clock::local_time() << " -> " << measure.getvalue()<<" "<<measure.getunit()<< std::endl;
// 	  	    std::cout << "[*] MaxiGauge " << boost::posix_time::second_clock::local_time() << " -> " << measure.getvalue()<<" "<<measure.getunit()<< std::endl;

	    
	  }
	  else  if(sensor==0)
	  { 
	    measuredValue measure;
	    for (int i=1;i<4;i++)
	    {
// 	       measure = this->maxiGauge->getPressure(i);
	      dbo::Transaction transaction(*session);
	      measure= measuredValue(1.0,"a",boost::posix_time::second_clock::local_time(), std::to_string(i));
	      this->session->add(static_cast<measuredDBValue*>(new measuredValue(measure.getvalue(),measure.getunit(),measure.getTimeStamp(), maxiGauge->getName()+std::to_string(i))));
	      transaction.commit();
	      std::cout << "[*] MaxiGauge " << boost::posix_time::second_clock::local_time() << " -> " << measure.getvalue()<<" "<<measure.getunit()<< std::endl;
	    }
	      
	   }
	  
	  
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
    
    PressureMonitor(boost::shared_ptr<dbo::Session> session,boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> maxiGauge) : session(session),Task(executionQueuePtr), maxiGauge(boost::dynamic_pointer_cast<MaxiGauge>(maxiGauge))
    {
      this->sensor = 0;    
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
// //       this->session->add(static_cast<measuredDBValue*>(new measuredValue(pressure.getvalue(),pressure.getunit(),pressure.getTimeStamp(), maxiGauge->getName())));
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