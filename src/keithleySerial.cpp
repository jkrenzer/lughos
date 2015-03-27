// #include "StdAfx.h"

#include <ostream>
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
    connection.character_size=boost::asio::serial_port_base::character_size(7); //TODO realy 7?
    connection.endOfLineRegExpr_='\r';
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


measuredValue kithleighSerial::getMeasure(bool force)
{
  if(!force &&!storedMeasure.getTimeStamp().is_not_a_date_time()&& storedMeasure.getTimeStamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(5))
  {
    return storedMeasure;
   }
    

  std::string s = this->inputOutput("READ?");
  static const boost::regex e("([\\d\\.+-]*[E][\\d\\.+-]*)([\\w]*)");

  boost::cmatch res;
  boost::regex_search(s.c_str(), res, e);
  double number = save_lexical_cast<double>(res[1],-1);

  s=res[2];
  measuredValue value;
  if(!s.empty() && number == 0)
  {
    value.setValue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
    value.setUnit(s);
    storedMeasure=value;
  }
    if(s.empty() && number == 0)
  {
    value.setValue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
    value.setUnit("");
    storedMeasure=value;    
  }
  else 
  {
    value.setValue(number);
    value.setUnit(s);
    value.setTimeStamp(boost::posix_time::second_clock::local_time());
    storedMeasure=value;
  }

  return value;
  
}
