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
  std::string s = this->inputOutput(":06030421402140\r\n");
  std::string debugs = s;
  s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
  s.erase( std::remove(s.begin(), s.end(), '\n'), s.end() );
  
  s.erase(0,1);
  int wordlen;
  int node;
  int chained;
  int type;
  float value=0;
  unsigned int protovalue;
//     std::cout<<s<<std::endl;
  std::stringstream(s.substr(0,2)) >> wordlen;
//     std::cout<<"wordlen: "<<wordlen<<std::endl;
  s.erase(0,2);
//       std::cout<<s<<std::endl;
  std::stringstream(s.substr(0,2)) >> node;
//       std::cout<<"node: "<<node<<std::endl;
  s.erase(0,2);
//       std::cout<<s<<std::endl;
  s.erase(0,2); //command "02"
  s.erase(0,2); //process 
  std::stringstream(s.substr(0,2)) >> type;
  s.erase(0,2); //parameter
//       std::cout<<s<<std::endl;
//   std::cout<<type<<std::endl;
  std::cout << "Bronkhorst answered: " << debugs << std::endl;
  std::cout << "I understood: Length" << ": "<< wordlen << " Node:" << node << " Type:" << type << std::endl;
  if(type==40)
  {
    std::stringstream(s.substr(0,4)) >> std::hex>>protovalue;  
    value=reinterpret_cast<float&>(protovalue);
  }
  
//    static const boost::regex e("(.*))");
// 
//   boost::cmatch res;
//   boost::regex_search(s.c_str(), res, e);
//   double number = save_lexical_cast<double>(res[0],-1);
// 
//   s=res[0];

// 
//     if(s.empty() && number == 0)
//   {
//     value.setvalue(save_lexical_cast<double>(s,std::numeric_limits<double>::signaling_NaN()));
//     value.setunit("sccm");
//     storedMeasure=value;    
//   }
//   else 
//   {
//     value.setvalue(number);
//     value.setunit("sccm");
//     value.settimestamp(boost::posix_time::second_clock::local_time());
//     storedMeasure=value;
//   }
  returnvalue.settimestamp(now);
  returnvalue.setvalue(value);
  returnvalue.setunit("sccm");
  return returnvalue;
  
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
  this->input(":050301000A52\r\n:050302010412\r\n:070304006000600F\r\n");
}
    

void bronkhorst::shutdownImplementation()
{
}
