// #include "StdAfx.h"

#include <ostream>
#include <sstream>
// #pragma comment(lib, "Setupapi.lib")
#include "bronkhorst.hpp"

#define Bronkhorst_100Percent 32000
#define Bronkhorst_signed_Int16_Min -23593
#define Bronkhorst_signed_Int16_Max 41942
#define Bronkhorst_unsigned_Int16_Max 65535

bronkhorst::bronkhorst() : capacity("capacity",this->ioService), flow("flow",this->ioService), setpoint("setpoint",this->ioService), controlMode("controlMode",this->ioService)
{
  this->capacity.getter(boost::bind(&bronkhorst::getMaxCapacity,this));
  this->capacity.expires(false);
  this->flow.getter(boost::bind(&bronkhorst::getFlow,this));
  this->setpoint.getter(boost::bind(&bronkhorst::getSetpoint,this));
  this->setpoint.setter(boost::bind(&bronkhorst::setSetpoint,this,_1));
}

bronkhorst::~bronkhorst(void)
{

}

void bronkhorst::memberDeclaration()
{
  this->addMember(this->capacity);
  this->addMember(this->flow);
  this->addMember(this->setpoint);
}


bronkhorstConnection::bronkhorstConnection()
{
    this->baud_rate=boost::asio::serial_port_base::baud_rate(38400);
    this->flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->endOfLineRegExpr_= boost::regex("\r\n");
    this->parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    this->stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
}



std::string bronkhorst::composeRequest(std::string query)
{
   SharedLock lock(this->mutex);
    std::string requestString="";
    requestString+=query;
      requestString+=std::string("\r\n");
    return requestString;
}

std::string bronkhorst::interpretAnswer(std::string s)
{  
   SharedLock lock(this->mutex);
	 return s;
}

measuredValue<double> bronkhorst::getSetpoint()
{
  LUGHOS_LOG_FUNCTION();
    boost::posix_time::ptime now= boost::posix_time::second_clock::local_time();
    measuredValue<double> returnvalue;
    bronkhorstMessage m1, a1;
    m1.setNode(3);
    m1.setType(4);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::Setpoint);
    m1.setParameterType(bronkhorstMessage::ParameterType::Integer);
    
    a1(this->inputOutput(m1));
  LUGHOS_LOG(lughos::log::SeverityLevel::informative) << "Sent: " << m1.toString() << " Received: " << a1.toString() << " Length:" << a1.getlength() << " Node:" << a1.getNode() << " Type:" << a1.getType() << " valueType:" << a1.getParameterType() << " value:" << a1.getValueString();
  double setpoint = 0.0;
  try
  {
    if(!a1.isStatusMessage())
    {
      SharedLock lock(this->mutex);
      int iSetpoint = 0;
      std::stringstream(a1.getValueString()) >>  iSetpoint;
      setpoint = ((float)iSetpoint/Bronkhorst_100Percent)*this->capacity;
      std::cout << "Setpoint is: " << iSetpoint << " of " << Bronkhorst_100Percent << " which calculates to " << setpoint << " of " << this->capacity.getValueAsString() << std::endl;
    }
    else
      LUGHOS_LOG(lughos::log::SeverityLevel::informative) << "Could not cast string to value! Setting value to zero.";
  }
  catch(std::exception& e)
  {
    LUGHOS_LOG(lughos::log::SeverityLevel::error) << "Exception while casting string to value! Setting value to zero.";
    setpoint=0.0;
  }
  
  returnvalue.setTimeStamp(now);
  returnvalue.setValue(setpoint);
  returnvalue.setUnit("sccm");
  return returnvalue;
  
}

measuredValue<double> bronkhorst::getMaxCapacity()
{
  LUGHOS_LOG_FUNCTION();
  bronkhorstMessage m1,a1;
  m1.setNode(3);
  m1.setType(4);
  m1.setProcess(1);
  m1.setParameter(bronkhorstMessage::Parameter::Capacity);
  m1.setParameterType(bronkhorstMessage::ParameterType::Float);
  a1(this->inputOutput(m1));
  measuredValue<double> tmp;
  tmp.setUnit("sccm");
  if(a1.hasValue())
    tmp.setValueFromString(a1.getValueString());
  else
    tmp.setValue(0.0); //TODO Throw exception which can beevaluated to connection status;
  return tmp;
}

measuredValue< bronkhorst::ControlMode > bronkhorst::getControlMode()
{
  LUGHOS_LOG_FUNCTION();
  bronkhorstMessage m1,a1;
  m1.setNode(3);
  m1.setType(4);
  m1.setProcess(1);
  m1.setParameter(bronkhorstMessage::Parameter::ControlMode);
  m1.setParameterType(bronkhorstMessage::ParameterType::Character);
  a1(this->inputOutput(m1));
  measuredValue<ControlMode> tmp;
  tmp.setUnit("");
  if(a1.hasValue())
    tmp.setValueFromString(a1.getValueString());
  else
    tmp.unset(); //TODO Throw exception which can beevaluated to connection status;
  return tmp;
}

measuredValue<double> bronkhorst::getFlow()
{
  LUGHOS_LOG_FUNCTION();
    boost::posix_time::ptime now= boost::posix_time::second_clock::local_time();
    measuredValue<double> returnvalue;
    bronkhorstMessage m1, a1;
    m1.setNode(3);
    m1.setType(4);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::Measure);
    m1.setParameterType(bronkhorstMessage::ParameterType::Integer);
    
    a1(this->inputOutput(m1));
 
  LUGHOS_LOG(log::SeverityLevel::debug) << "Sent: " << m1.toString() << "Received: " << a1.toString() << " Length:"<< a1.getlength() << " Node:" << a1.getNode() << " Type:" << a1.getType() << " valueType:" << a1.getParameterType() << " value:" << a1.getValueString();
  double setpoint = 0.0;
  try
  {
    if(!a1.isStatusMessage())
    {
      SharedLock lock(this->mutex);
      int32_t iSetpoint = 0;
      std::stringstream(a1.getValueString()) >>  iSetpoint;
      iSetpoint = iSetpoint > Bronkhorst_signed_Int16_Max ? iSetpoint - Bronkhorst_unsigned_Int16_Max : iSetpoint; //Calculate strange Bronkhorst signed-int16-definition o.0
      setpoint = ((double)(iSetpoint)/Bronkhorst_100Percent)*this->capacity;
      LUGHOS_LOG(log::SeverityLevel::debug) << "Measured flow is: " << iSetpoint << " of " << Bronkhorst_100Percent << " which calculates to " << setpoint << " of " << this->capacity.getValueAsString();
    }
    else
      LUGHOS_LOG(log::SeverityLevel::informative) << "Could not cast string to value! Setting value to zero.";
  }
  catch(std::exception& e)
  {
    LUGHOS_LOG(lughos::log::SeverityLevel::error) << "Exception while casting string to value! Setting value to zero.";
    setpoint=0.0;
  }
  
  returnvalue.setTimeStamp(now);
  returnvalue.setValue(setpoint);
  returnvalue.setUnit("sccm");
  return returnvalue;
  
}


void bronkhorst::setSetpoint(measuredValue<double> value)
{
  LUGHOS_LOG_FUNCTION();
  SharedLock lock(this->mutex);
  if(value == std::numeric_limits<float>::infinity())
    return ;
  else if (value > this->capacity) value = this->capacity;
  else if (value < 0) value = 0;
  int iSetpoint = (value/this->capacity)*Bronkhorst_100Percent;
  bronkhorstMessage m1, a1;
  std::string s;
  m1.setNode(3);
    m1.setType(1);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::Setpoint);
    m1.setParameterType(bronkhorstMessage::ParameterType::Integer);
    m1.setValueString(iSetpoint);
    lock.unlock();
    s = this->inputOutput(m1);
    a1(s);
    LUGHOS_LOG(lughos::log::SeverityLevel::debug) << "Setting flow to " << iSetpoint << " Sent: " << m1.toString() << " Received: " << a1.toString() << "Length" << ": "<< a1.getlength() << " Node:" << a1.getNode() << " Type:" << a1.getType() << " statusCode:" << a1.getStatusCode() << " Corresponds to:" << a1.getStatusSubjectFirstByte();
}

void bronkhorst::setControlMode(measuredValue<ControlMode> value)
{
  LUGHOS_LOG_FUNCTION();
  SharedLock lock(this->mutex);
  bronkhorstMessage m1, a1;
  std::string s;
  m1.setNode(3);
    m1.setType(1);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::ControlMode);
    m1.setParameterType(bronkhorstMessage::ParameterType::Character);
    m1.setValueString(value);
    lock.unlock();
    s = this->inputOutput(m1);
    a1(s);
    LUGHOS_LOG(lughos::log::SeverityLevel::debug) << "Setting flow to " << value.getString() << " Sent: " << m1.toString() << " Received: " << a1.toString() << "Length" << ": "<< a1.getlength() << " Node:" << a1.getNode() << " Type:" << a1.getType() << " valueType:" << a1.getParameterType() << " value:" << a1.getValueString();
}

void bronkhorst::initImplementation()
{
  LUGHOS_LOG_FUNCTION();
  LUGHOS_LOG(lughos::log::SeverityLevel::debug) << "Init string 1. Sent: :050301000A49 Received: " << this->inputOutput(":050301000A49"); //Initialize device and wait 2 secs to settle
  boost::this_thread::sleep(boost::posix_time::seconds(2));
  LUGHOS_LOG(lughos::log::SeverityLevel::debug) << "Init string 2. Sent: :050301000502 Received: " << this->inputOutput(":050301000502");
  LUGHOS_LOG(lughos::log::SeverityLevel::debug) << "Init string 3. Sent: :050301000A52 Received: " << this->inputOutput(":050301000A52");
  this->input(":050302010412");
  LUGHOS_LOG(lughos::log::SeverityLevel::debug) << "Init string 4. Sent: :050302010400 No answer expected."; //Set to RS232 Control
//   this->inputOutput(":070304006000600F");

}

bool bronkhorst::isConnectedImplementation()
{
  SharedLock lock(this->mutex);
  this->capacity.refresh();
  if(this->capacity.getValue() > 0)
    return true;
  else
    return false;
}

void bronkhorst::shutdownImplementation()
{
}
