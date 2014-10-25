// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "serialAsync.hpp"
#include "kithleighSerial.hpp"

kithleighSerial::kithleighSerial()
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

template <class T> void kithleighSerial::setDefaultImpl(T& connection)
{
}
template <> void kithleighSerial::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(7);
    connection.end_of_line_char_='\r';
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
    
}



kithleighSerial::~kithleighSerial(void)
{

}



std::string kithleighSerial::composeRequest(std::string query)
{

    std::string requestString="";
    requestString+=query;
    requestString+=std::string("\r");

    return requestString;
  
}


void kithleighSerial::set_default()
{

}
std::string kithleighSerial::interpretAnswer(std::string s)
{       
  return s;  

}

void kithleighSerial::initImplementation()
{
}
    

void kithleighSerial::shutdownImplementation()
{
}

