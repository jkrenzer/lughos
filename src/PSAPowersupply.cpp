// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "PSAPowersupply.hpp"
#include <cmath>


PSAPowersupply::PSAPowersupply() : state("state"), current("current"), voltage("voltage"), temperature("temperature")
{
  this->state.getter(boost::bind(&PSAPowersupply::get_state,this));
  this->state.setter(boost::bind(&PSAPowersupply::set_state,this,_1));
  this->current.getter(boost::bind(&PSAPowersupply::get_current,this));
  this->voltage.getter(boost::bind(&PSAPowersupply::get_voltage,this));
  this->temperature.getter(boost::bind(&PSAPowersupply::get_temperature,this));
}

void PSAPowersupply::memberDeclaration()
{
  addMember(this->state);
  addMember(this->current);
  addMember(this->voltage);
  addMember(this->temperature);
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

PSAPowersupplyConnection::PSAPowersupplyConnection()
{
  
    this->baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    this->flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->endOfLineRegExpr_="\\$";
    this->parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    this->stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);

}

PSAPowersupply::~PSAPowersupply(void)
{

}

std::string PSAPowersupply::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
//     if (query!="\x05")
//       requestString+=std::string("\r");

    return requestString;
  
}

measuredValue<bool> PSAPowersupply::get_state()
{
  measuredValue<bool> tmp;
  tmp.setValue(this->get_voltage() > 0.5);
  return tmp ;
}

void PSAPowersupply::set_state(bool state = 1)
{
  if(state)
    this->inputOutput("\x10");
  else
    this->inputOutput("\x11");
}


measuredValue<double> PSAPowersupply::get_current()
{
//   exp_lenght=4;
 measuredValue<double> value;
 std::string answer = this->inputOutput("\x02");
  if (answer.size() < 3)
   return measuredValue<double>();
 boost::regex expr("(\\d*)(\\d\\d)\\$"); 
 boost::smatch result;
 boost::regex_search( answer.cbegin(),answer.cend(),result,expr);
 value.setValueFromString(result[1] + '.' + result[2]);
 value.setUnit("A");
 return value;
}

measuredValue<double> PSAPowersupply::get_voltage()
{
//  exp_lenght=4;
 measuredValue<double> value;
 std::string answer = this->inputOutput("\x01");
  if (answer.size() < 3)
   return measuredValue<double>();
 boost::regex expr("(\\d*)(\\d\\d)\\$"); 
 boost::smatch result;
 boost::regex_search( answer.cbegin(),answer.cend(),result,expr);
 value.setValueFromString(result[1] + '.' + result[2]);
 value.setUnit("V");
 return value;
  
}

measuredValue<double> PSAPowersupply::get_temperature()
{
//  exp_lenght=4;
 measuredValue<double> value;
 std::string answer = this->inputOutput("\x03");
 boost::regex expr("(\\d*)\\$"); 
 boost::smatch result;
 boost::regex_search( answer.cbegin(),answer.cend(),result,expr);
 value.setValueFromString(result[1]);
 value.setUnit("°C");
 return value;
  
}

std::string PSAPowersupply::interpretAnswer(std::string s)
{  
  return s;     
}


void PSAPowersupply::initImplementation()
{

}
    

void PSAPowersupply::shutdownImplementation()
{
}

bool PSAPowersupply::isConnectedImplementation()
{
  return this->get_temperature().isSet();
}
