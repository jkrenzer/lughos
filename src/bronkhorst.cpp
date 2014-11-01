// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialAsync.hpp"
#include "bronkhorst.hpp"


bronkhorst::bronkhorst()
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

template <class T> void bronkhorst::setDefaultImpl(T& connection)
{
}



bronkhorst::~bronkhorst(void)
{

}

template <> void bronkhorst::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(38400);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);
    connection.end_of_line_char_='\n';
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
  std::string s = this->inputOutput(":06030421402140\r\n");
  static const boost::regex e("(.*))");

  boost::cmatch res;
  boost::regex_search(s.c_str(), res, e);
  double number = save_lexical_cast<double>(res[0],-1);

  s=res[0];
  measuredValue value;

    if(s.empty() && number == 0)
  {
    value.setvalue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
    value.setunit("sccm");
    storedMeasure=value;    
  }
  else 
  {
    value.setvalue(number);
    value.setunit("sccm");
    value.settimestamp(boost::posix_time::second_clock::local_time());
    storedMeasure=value;
  }

  return value;
  
}

std::string bronkhorst::set_flow(float value)
{
  
  if(value == std::numeric_limits<float>::infinity())return "Bad flow request.";
   int* input_int = (int*)&value;
   
    std::ostringstream request;
    request << std::hex << input_int;
    std::string s = request.str();
    
      
  return this->inputOutput(":0803022143"+s+"\r\n");
  
}

void bronkhorst::initImplementation()
{
  this->input(":050302010412\r\n070304006000600F\r\n");
}
    

void bronkhorst::shutdownImplementation()
{
}
