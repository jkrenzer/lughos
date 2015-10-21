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

#ifndef TEST_GUI_MONITORING2_HPP
#define TEST_GUI_MONITORING2_HPP

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include "RFG.hpp"
#include "bronkhorst.hpp"
#include "measuredValue.hpp"
#include "jobQueue.hpp"
#include "measuredDBValue.hpp"

using namespace lughos;

class PressureMeasurement : public measuredValue<double>
{
  
};

class BronkhorstTest : public Task
{
  protected:
  
    boost::shared_ptr<bronkhorst> horst;
    boost::shared_ptr<Wt::Dbo::Session> session;
  
    virtual void run()
    { 
     if( horst->isConnected())
      { 
	try{
	  measuredValue<double> measure = this->horst->flow;

	  Wt::Dbo::Transaction transaction(*session);
	  this->session->add(static_cast<measuredDBValue<double>*>(&measure));
	  transaction.commit();
	  std::cout << "[*] Bronkhorst " << boost::posix_time::second_clock::local_time() << " -> " << measure.getValue()<<" "<<measure.getUnit()<< std::endl;
	}
	catch(...)
	{
	  
	}
     }
       
    }
    
        virtual void init()
    {
//      if( keithley->isConnected())
//       {
// 	try{
// 	this->keithley->input("*RST");
// 	  }
// 	catch(...)
// 	{
// 	  
// 	}
//      }
    }
public:
  
    BronkhorstTest(boost::shared_ptr<Wt::Dbo::Session> session,boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> horst) : session(session),Task(executionQueuePtr), horst(boost::dynamic_pointer_cast<bronkhorst>(horst))
    {
    
    }
  
};

class RFGTest : public Task
{
  protected:
  
    boost::shared_ptr<RFG> rfg;
    boost::shared_ptr<Wt::Dbo::Session> session;
    int sensor;
  
    virtual void run()
    { 
     if( rfg->isConnected())
      { 
	try{
	  measuredValue<double> measure = this->rfg->get_channel(sensor);

	  Wt::Dbo::Transaction transaction(*session);
	  this->session->add(static_cast<measuredDBValue<double>*>(new measuredValue<double>(measure.getValue(),measure.getUnit(),measure.getTimeStamp(), rfg->getName())));
	  transaction.commit();
// 	  std::cout << "[*] Bronkhorst " << boost::posix_time::second_clock::local_time() << " -> " << measure.getvalue()<<" "<<measure.getunit()<< std::endl;
	}
	catch(...)
	{
	  
	}
     }
       
    }
    
        virtual void init()
    {
//      if( keithley->isConnected())
//       {
// 	try{
// 	this->keithley->input("*RST");
// 	  }
// 	catch(...)
// 	{
// 	  
// 	}
//      }
    }
public:
  
    RFGTest(boost::shared_ptr<Wt::Dbo::Session> session,boost::shared_ptr< boost::asio::io_service > executionQueuePtr, boost::shared_ptr<Device> rfg, int sensor) : session(session),Task(executionQueuePtr), rfg(boost::dynamic_pointer_cast<RFG>(rfg))
    {
          this->sensor = sensor; 
    }
  
};

#endif