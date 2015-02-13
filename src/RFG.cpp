// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "serialAsync.hpp"
#include "RFG.hpp"

RFG::RFG()
{
  for (int i=0;i<8;i++)
    {
      channel_output[i].setunitvalue(0,"");
    }
  SplineTransformation::XToYMap& x2y = unitsToVoltage.valueMap.left;
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0000", NULL, 0), 0.645));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0100", NULL, 0), 3.270));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0200", NULL, 0), 6.373));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0300", NULL, 0), 9.489));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0400", NULL, 0),12.592));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0500", NULL, 0),15.702));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0600", NULL, 0),18.817));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0700", NULL, 0),21.919));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0800", NULL, 0),25.028));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0900", NULL, 0),28.137));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0A00", NULL, 0),31.247));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0B00", NULL, 0),34.355));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0C00", NULL, 0),37.471));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0D00", NULL, 0),39.489));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0E00", NULL, 0),39.490));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0xFFFF", NULL, 0), 39.5));
  unitsToVoltage.init();
  x2y = unitsToCurrent.valueMap.left;
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0000", NULL, 0), 0.0));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x7FFF", NULL, 0), 2.5));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0xFFFF", NULL, 0), 5.0));
  unitsToCurrent.init();
  x2y = unitsToPower.valueMap.left;
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0000", NULL, 0), 0.0));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x7FFF", NULL, 0), 95.0));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0xFFFF", NULL, 0), 190.0));
  unitsToPower.init();
}

template <class T, class S> T save_lexical_cast(S& source, T saveDefault)
{
  try
  {
    return boost::lexical_cast<T>(source);
  }
  catch(boost::bad_lexical_cast e)
  {
    return saveDefault;
  }
  
}



RFGConnection::RFGConnection(boost::shared_ptr< boost::asio::io_service > io_service): serialAsync(io_service), Connection<serialContext>(io_service)
{
  std::cout << "--------------------->>>>>>>>>>>>>>>>> !!!!!!!!!!!!!!!!! <<<<<<<<<<<<<<<<<<<------------------------" << std::endl << std::endl << std::endl;
    this->baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    this->flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    this->character_size=boost::asio::serial_port_base::character_size(8);//unconfirmed
    this->endOfLineRegExpr_= boost::regex("\x0d");//unconfirmed
    this->parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    this->stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
    
}


RFG::~RFG(void)
{

}

std::string RFG::composeRequest(std::string query)
{

    std::string requestString="";
    requestString+=query;
//     requestString+=std::string("\r");
    std::cout<<"R: "<<requestString<<std::endl;
    return requestString;
  
}



std::string RFG::interpretAnswer(std::string s)
{ 
//   s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
//   s.erase( std::remove(s.begin(), s.end(), '\n'), s.end() );
      std::cout<<"Answer: "<<s<<std::endl;
  return s;
}

void RFG::power_supply_mode()
{
 if(bccMode==false)this->inputOutput("A"); 
}

void RFG::bcc_mode()
{
 if(bccMode==true)this->inputOutput("B"); 
}

void RFG::use_voltage_controler()
{
 this->inputOutput("F");
 this->controllerMode = ControllerMode::Voltage;
}

void RFG::use_current_controler()
{
 this->inputOutput("G");
 this->controllerMode = ControllerMode::Current;
}

void RFG::use_power_controler()
{
 this->inputOutput("H");
 this->controllerMode = ControllerMode::Power;
}

void RFG::switch_on()
{
  this->inputOutput("N");
}

void RFG::switch_off()
{
  this->inputOutput("O");
}

float RFG::getLimitMaxVoltage()
{
  return this->maxVoltage.getvalue();
}

float RFG::getLimitMinVoltage()
{
  return this->minVoltage.getvalue();
}

float RFG::getLimitMaxCurrent()
{
  return this->maxCurrent.getvalue();
}

float RFG::getTargetValue()
{
  return this->maxPower.getvalue();
}

std::string RFG::floatToBinaryStr(float f, SplineTransformation& transformation)
{
  uint16_t tmp = transformation.yToX(f); //Convert between arbitrary units and volts
  char buffer[sizeof(uint16_t)];
  memcpy(buffer,&tmp,sizeof(uint16_t)); //Transfer binary data to char-array
  std::string rawRequest(buffer,sizeof(uint16_t)); 
  std::string request(rawRequest.rbegin(),rawRequest.rend()); //Reverse to keep the endian straight!
  return request;
}


float RFG::set_voltage_max(float f)
{
//   if(voltage_min>f) return 0;
  std::stringstream stream;
//   std::cout << "#### DEBUG: " << f << " - " << unitsToVoltage.yToX(f) << " - " << (uint16_t) unitsToVoltage.yToX(f) << " - " << stream.str() << std::endl;
//   union Request { char chars[2]; uint16_t value;  } request;
//   request.value = (uint16_t) (unitsToVoltage.yToX(f));
  std::string answer = this->inputOutput(std::string("\x00")+"U"+floatToBinaryStr(f,unitsToVoltage)+"\r",boost::regex("A\\w\\w\\w\\w"));
  boost::regex exp1("A(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value; 
}


float RFG::set_voltage_min(float  f)
{
//   if(voltage_max<f) return 0;
  std::stringstream stream;
  
  std::string answer = this->inputOutput(std::string("\x00")+"M"+floatToBinaryStr(f,unitsToVoltage)+"\r",boost::regex("B\\w\\w\\w\\w"));
  boost::regex exp1("B(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value; 
}

float RFG::set_current_lim(float  f)
{
  std::stringstream stream;
  std::string answer = this->inputOutput(std::string("\x00")+"I"+floatToBinaryStr(f,unitsToCurrent)+"\r",boost::regex("C\\w\\w\\w\\w"));
  boost::regex exp1("C(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  return value; 
}

int RFG::set_target_value(float f)
{
  std::stringstream stream;
  SplineTransformation* trafo;
  switch (this->controllerMode)
  {
    case ControllerMode::Voltage:
      trafo = &this->unitsToVoltage;
      break;
    case ControllerMode::Current:
      trafo = &this->unitsToCurrent;
      break;
    case ControllerMode::Power:
      trafo = &this->unitsToPower;
      break;
    default:
      trafo = &this->unitsToVoltage;
      break;
  }
  std::string answer = this->inputOutput(std::string("\x00")+"P"+floatToBinaryStr(f,*trafo)+"\r",boost::regex("D\\w\\w\\w\\w"));
  boost::regex exp1("D(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
 return value; 
}

bool RFG::readoutChannels()
{
  int value=0;
  std::stringstream stream;
//   std::cout<<"S: "<<s<<std::endl;
  std::string s = this->inputOutput("\x00\x20\x00");
//   this->inputOutput("\r");
  std::cout<<"S: "<<s<<std::endl;
  boost::regex exp1("....(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)");
  boost::cmatch res1;
  boost::regex_search(s.c_str(), res1, exp1);
  for(int i =0;i<8;i++)
  {
    stream <<res1[i+1];
    stream >> value;
    channel_output[i].setvalue(value);
    if (this->controllerMode==ControllerMode::Voltage)channel_output[i].setunit("V");
    if (this->controllerMode==ControllerMode::Current)channel_output[i].setunit("I");
    if (this->controllerMode==ControllerMode::Power)channel_output[i].setunit("Watt");
    channel_output[i].settimestamp(boost::posix_time::second_clock::local_time());
  }
  
  return true;
}

bool RFG::readoutSetting(measuredValue& value, std::string unit, std::string controlChar, std::string answerChar, SplineTransformation& transformation)
{
  std::string s = this->inputOutput(std::string("\x00")+controlChar+controlChar+controlChar+controlChar+std::string("\r"),boost::regex(answerChar + std::string("\\w\\w\\w\\w"))); //Provoke Error to get setting
  boost::regex exp1(answerChar + std::string("(\\w\\w\\w\\w)"));
  boost::cmatch res1;
  boost::regex_search(s.c_str(), res1, exp1);
  unsigned int valueTemp = 0;
  std::stringstream stream;
  if(!res1.empty())
  {
    stream << res1[1];
    stream >> std::hex >> valueTemp;
    value.setvalue(transformation.xToY(valueTemp));
    std::cout << "RECEIVED: " << res1[1] << " - " << valueTemp << " -- " << exp1.str() << " - " << answerChar << " === " << value.getStringValue() << unit <<  std::endl;
    value.setunit(unit);
  }
}

bool RFG::readout()
{
  bool result;
  try
  {
    SplineTransformation* trafo;
    std::string targetUnit;
    switch (this->controllerMode)
  {
    case ControllerMode::Voltage:
      trafo = &this->unitsToVoltage;
      targetUnit = std::string("V");
      break;
    case ControllerMode::Current:
      trafo = &this->unitsToCurrent;
      targetUnit = std::string("A");
      break;
    case ControllerMode::Power:
      trafo = &this->unitsToPower;
      targetUnit = std::string("W");
      break;
    default:
      trafo = &this->unitsToVoltage;
      targetUnit = std::string("V");
      break;
  }
    result = this->readoutChannels() && result;
    result = this->readoutSetting(this->maxVoltage,"V","U","A",this->unitsToVoltage) && result;
    result = this->readoutSetting(this->minVoltage,"V","M","B",this->unitsToVoltage) && result;
    result = this->readoutSetting(this->maxCurrent,"A","I","C",this->unitsToCurrent) && result;
    result = this->readoutSetting(this->maxPower,targetUnit,"P","D",*trafo) && result;
  }
  catch(...)
  {
    return false;
  }
  return result;
}


measuredValue RFG::get_channel(int i, bool force)
{
 if(!force &&!channel_output[0].gettimestamp().is_not_a_date_time()&& channel_output[0].gettimestamp() > boost::posix_time::second_clock::local_time()-boost::posix_time::seconds(3))
  {
    return channel_output[i];
  }
  
  this->readout();
 return channel_output[i];
}


void RFG::initImplementation()
{
if(isConnected())
  std::cout << "RFG answered correctly! YAY!!!!" << std::endl;
this->bccMode=true;
this->controllerMode=ControllerMode::Voltage;
  
}
    
bool RFG::isConnectedImplementation()
{
  std::string initStr = this->inputOutput(std::string("\x00")+std::string("AF")+std::string("\r"),boost::regex("@"));
  this->connected = initStr == std::string("@");
  return this->connected;
}


void RFG::shutdownImplementation()
{
}

