// #include "StdAfx.h"

#include <ostream>
#include <sstream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialAsync.hpp"
#include "bronkhorst.hpp"


bronkhorst::bronkhorst()
{
  set_default();
}

template <class T> void bronkhorst::setDefaultImpl(Connection<T>& connection)
{
}



bronkhorst::~bronkhorst(void)
{

}

template <> void bronkhorst::setDefaultImpl< serialContext > (Connection<serialContext>& connection)
{
    std::cout << "########################## Setting connection-parameters" << std::endl;
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(38400);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);
    connection.end_of_line_char_='\r';
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);

    
}

std::string bronkhorst::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
      requestString+=std::string("\r\n");

    return requestString;
  
}

std::string bronkhorst::interpretAnswer(std::string s)
{  

	 return s;   
    

}

void bronkhorst::set_default()
{
   this->setDefaultImpl(*(this->connection.get()));
}

measuredValue bronkhorst::get_value()
{
    boost::posix_time::ptime now= boost::posix_time::second_clock::local_time();
    measuredValue returnvalue;
    bronkhorstMessage m1, m2, a1, a2;
    m1.setNode(3);
    m1.setType(4);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::Setpoint);
    m1.setParameterType(bronkhorstMessage::ParameterType::Integer);
    
    a1(this->inputOutput(m1));
 
  std::cout << "I asked: " << m1.toString() << std::endl;
  std::cout << "I understood: Length" << ": "<< a1.getlength() << " Node:" << a1.getNode() << " Type:" << a1.getType() << " valueType:" << a1.getParameterType() << " value:" << a1.getValueString() << std::endl;
  double setpoint;
  try
  {
    if(!a1.isStatusMessage())
    {
      int iSetpoint;
      std::stringstream(a1.getValueString()) >>  iSetpoint;
      setpoint = ((float)iSetpoint/32767.0)*this->maxCapacity;
      std::cout << "Setpoint is: " << iSetpoint << " of 32767 which calculates to " << setpoint << " of " << this->maxCapacity << std::endl;
    }
    else
      std::cout << "Could not cast string to value! Setting value to zero." << std::endl;
  }
  catch(std::exception& e)
  {
    std::cout << "Could not cast string to value! Setting value to zero." << std::endl;
    setpoint=0.0;
  }
  
  returnvalue.settimestamp(now);
  returnvalue.setvalue(setpoint);
  returnvalue.setunit("sccm");
  return returnvalue;
  
}

std::string bronkhorst::set_flow(float value)
{
  
  if(value == std::numeric_limits<float>::infinity())return "Bad flow request.";
  int iSetpoint = (value/this->maxCapacity)*32767;
  bronkhorstMessage m1;
  std::string s;
  m1.setNode(3);
    m1.setType(2);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::Setpoint);
    m1.setParameterType(bronkhorstMessage::ParameterType::Integer);
    m1.setValueString(iSetpoint);
    s = this->inputOutput(m1);
    std::cout << "I told the bronkhorst to set flow to " << iSetpoint << " (" << m1.toString() << ")" << std::endl;
  return s;
  
}

void bronkhorst::initImplementation()
{
  this->inputOutput(":050301000A52\r\n");
  this->inputOutput("050302010412\r\n");
  this->inputOutput(":070304006000600F\r\n");
  this->maxCapacity = 1.0;
  bronkhorstMessage m1,a1;
  m1.setNode(3);
    m1.setType(4);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::Capacity);
    m1.setParameterType(bronkhorstMessage::ParameterType::Float);
    a1(this->inputOutput(m1));
    if(!a1.isStatusMessage())
    {
      std::stringstream(a1.getValueString()) >> this->maxCapacity;
      std::cout << "MAXCAPACITY: " << this->maxCapacity;
    }
    else
      std::cout << "UNABLE TO SET CAPACITY! <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    

}

bool bronkhorst::isConnectedImplementation()
{
  return true;
}

void bronkhorst::shutdownImplementation()
{
}
