// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "PSANetzteil.hpp"


PSANetzteil::PSANetzteil()
{
  set_default();
  
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

template <class T> void PSANetzteil::setDefaultImpl(T& connection)
{
}

template <> void PSANetzteil::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);
    connection.endOfLineRegExpr_='$';
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);

}

PSANetzteil::~PSANetzteil(void)
{

}

std::string PSANetzteil::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
//     if (query!="\x05")
//       requestString+=std::string("\r");

    return requestString;
  
}

   void PSANetzteil::set_default()
{
   this->setDefaultImpl(*(this->connection.get()));
}

void PSANetzteil::off()
{

  this->input("\x11");
//     is_on=false;
  
}

void PSANetzteil::on()
{

  this->input("\x10");
//     is_on=true;
  
}

std::string PSANetzteil::get_current()
{
//   exp_lenght=4;
 return this->inputOutput("\x02");
  
}

std::string PSANetzteil::get_voltage()
{
//  exp_lenght=4;
 return this->inputOutput("\x01");
  
}

std::string PSANetzteil::get_temperature()
{
//  exp_lenght=4;
 return this->inputOutput("\x03");
  
}

std::string PSANetzteil::interpretAnswer(std::string s)
{  
  return s;     
}


void PSANetzteil::initImplementation()
{

}
    

void PSANetzteil::shutdownImplementation()
{
}