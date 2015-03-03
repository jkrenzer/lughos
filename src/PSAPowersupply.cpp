// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "PSAPowersupply.hpp"
#include <cmath>


PSAPowersupply::PSAPowersupply()
{
 
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

PSAPowersupplyConnection::PSAPowersupplyConnection(boost::shared_ptr< boost::asio::io_service > io_service): serialAsync(io_service), Connection<serialContext>(io_service)
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

void PSAPowersupply::off()
{

  this->inputOutput("\x11");
//     is_on=false;
  
}

void PSAPowersupply::on()
{

  this->inputOutput("\x10");
//     is_on=true;
  
}

measuredValue PSAPowersupply::get_current()
{
//   exp_lenght=4; 
 measuredValue value;
 std::string answer = this->inputOutput("\x02");
  if (answer.size() < 3)
   return measuredValue();
 std::string::reverse_iterator it = answer.rend();
 it+=2;
 answer.insert(it.base(),'.');
 value.setStringValue(answer);
 value.setunit("A");
 return value;
}

measuredValue PSAPowersupply::get_voltage()
{
//  exp_lenght=4;
 measuredValue value;
 std::string answer = this->inputOutput("\x01");
  if (answer.size() < 3)
   return measuredValue();
 std::string::reverse_iterator it = answer.rend();
 it+=2;
 answer.insert(it.base(),'.');
 value.setStringValue(answer);
 value.setunit("V");
 return value;
  
}

measuredValue PSAPowersupply::get_temperature()
{
//  exp_lenght=4;
 measuredValue value;
 std::string answer = this->inputOutput("\x03");
 boost::regex expr("(\d*)\\$"); 
 boost::smatch result;
 boost::regex_search(answer.begin(),answer.end(),result,expr);
 value.setStringValue(result[1]);
 value.setunit("°C");
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
  return !isnan(this->get_temperature());
}
