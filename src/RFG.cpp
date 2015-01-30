// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "serialAsync.hpp"
#include "RFG.hpp"

RFG::RFG()
{
  set_default();
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

template <class T> void RFG::setDefaultImpl(T& connection)
{
}

template <> void RFG::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);//unconfirmed
    connection.end_of_line_char_='\r';//unconfirmed
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
    for (int i=0;i<8;i++)
    {
      channel_output[i].setunitvalue(0,"");
    }
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

void RFG::set_default()
{ 
   this->setDefaultImpl(*(this->connection.get()));
}

void RFG::power_supply_mode()
{
 if(mode==false)this->input("A"); 
}

void RFG::bcc_mode()
{
 if(mode==true)this->input("B"); 
}

void RFG::use_voltage_controler()
{
 this->input("F");
 this->controler=0;
}

void RFG::use_current_controler()
{
 this->input("G");
 this->controler=1;
}

void RFG::use_power_controler()
{
 this->input("H");
 this->controler=2;
}

void RFG::switch_on()
{
  this->input("N");
}

void RFG::switch_off()
{
  this->input("O");
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

float RFG::set_voltage_max(float f)
{
//   if(voltage_min>f) return 0;
  std::stringstream stream;
  std::cout << "#### DEBUG: " << f << " - " << unitsToVoltage.yToX(f) << " - " << (int) unitsToVoltage.yToX(f) << " - " << stream.str() << std::endl;
  union requestUnion
  {
    uint16_t number;
    char chars[2];
  } request;
  request.chars = "XX";
  request.number = unitsToVoltage.yToX(f);
  std::string answer = this->inputOutput(std::string("\x00")+"U"+std::string(request.chars)+"\r",boost::regex("A\\w\\w\\w\\w"));
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
  union requestUnion
  {
    uint16_t number;
    char chars[2];
  } request;
  request.chars = "XX";
  request.number = unitsToVoltage.yToX(f);
  std::string answer = this->inputOutput(std::string("\x00")+"M"+std::string(request.chars)+"\r",boost::regex("B\\w\\w\\w\\w"));
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
  union requestUnion
  {
    uint16_t number;
    char chars[2];
  } request;
  request.chars = "XX";
  request.number = unitsToVoltage.yToX(f);
  std::string answer = this->inputOutput(std::string("\x00")+"I"+std::string(request.chars)+"\r",boost::regex("C\\w\\w\\w\\w"));
  boost::regex exp1("C(\\w\\w\\w\\w)");
  boost::cmatch res1;
  boost::regex_search(answer.c_str(), res1, exp1);
  int value;
  stream << res1[1];
  stream >> std::hex >> value;
  return value; 
}

int RFG::set_power_lim(float f)
{
  std::stringstream stream;
  union requestUnion
  {
    uint16_t number;
    char chars[2];
  } request;
  request.chars = "XX";
  request.number = unitsToVoltage.yToX(f);
  std::string answer = this->inputOutput(std::string("\x00")+"P"+std::string(request.chars)+"\r",boost::regex("D\\w\\w\\w\\w"));
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
  boost::posix_time::ptime now= boost::posix_time::second_clock::local_time();
  std::cout<<"S: "<<s<<std::endl;
  boost::regex exp1("....(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)");
  boost::cmatch res1;
  boost::regex_search(s.c_str(), res1, exp1);
  for(int i =0;i<8;i++)
  {
    stream <<res1[i+1];
    stream >> value;
    channel_output[i].setvalue(value);
    if (controler==0)channel_output[i].setunit("V");
    if (controler==1)channel_output[i].setunit("I");
    if (controler==2)channel_output[i].setunit("Watt");
    channel_output[i].settimestamp(now);
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
    value.setvalue(unitsToVoltage.xToY(valueTemp));
    std::cout << "RECEIVED: " << res1[1] << " - " << valueTemp << " -- " << exp1.str() << " - " << answerChar << " === " << value.getStringValue() <<  std::endl;
    value.setunit(unit);
  }
}

bool RFG::readout()
{
  bool result;
  try
  {
    result = this->readoutChannels() && result;
    result = this->readoutSetting(this->maxVoltage,"V","U","A",this->unitsToVoltage) && result;
    result = this->readoutSetting(this->minVoltage,"V","M","B",this->unitsToVoltage) && result;
  }
  catch(...)
  {
    return false;
  }
  return result;
}


measuredValue RFG::get_channel(int i, bool force)
{
 if(!force &&!channel_output[0].gettimestamp().is_not_a_date_time()&& channel_output[0].gettimestamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(3))
  {
    return channel_output[i];
  }
  
  this->readout();
 return channel_output[i];
}


void RFG::initImplementation()
{
std::string initStr = this->inputOutput(std::string("\x00")+std::string("AF")+std::string("\r"),boost::regex("@"));
if(initStr == std::string("@"))
  std::cout << "RFG answered correctly! YAY!!!!" << std::endl;
this->mode=true;
controler =0;
}
    

void RFG::shutdownImplementation()
{
}

