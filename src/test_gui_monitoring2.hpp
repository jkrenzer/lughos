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

class PressureMeasurement : public measuredValue
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
	  measuredValue measure = this->horst->get_value();

	  Wt::Dbo::Transaction transaction(*session);
	  this->session->add(static_cast<measuredDBValue*>(new measuredValue(measure.getvalue(),measure.getunit(),measure.gettimestamp(), horst->getName())));
	  transaction.commit();
	  std::cout << "[*] Bronkhorst " << boost::posix_time::second_clock::local_time() << " -> " << measure.getvalue()<<" "<<measure.getunit()<< std::endl;
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
	  measuredValue measure = this->rfg->get_channel(sensor);

	  Wt::Dbo::Transaction transaction(*session);
	  this->session->add(static_cast<measuredDBValue*>(new measuredValue(measure.getvalue(),measure.getunit(),measure.gettimestamp(), rfg->getName())));
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