// #include "StdAfx.h"

#include <ostream>
#include <sstream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialAsync.hpp"
#include "bronkhorst.hpp"


bronkhorst::bronkhorst()
{
  set_default();
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
    bronkhorstMessage m1, m2, a1, a2;
    m1.setNode(3);
    m1.setType(4);
    m1.setProcess(1);
    m1.setParameter(bronkhorstMessage::Parameter::Setpoint);
    m1.setParameterType(bronkhorstMessage::ParameterType::Integer);
    m2.setNode(3);
    m2.setType(4);
    m2.setProcess(1);
    m2.setParameter(bronkhorstMessage::Parameter::Capacity);
    m2.setParameterType(bronkhorstMessage::ParameterType::Float);
    a1(this->inputOutput(m1));
    a2(this->inputOutput(m2));
//     std::string setpointStr = this->inputOutput(":06030401210121\r\n");
//     std::string capacityStr = this->inputOutput(":060304012D012D\r\n");
//   std::string debugs = setpointStr;
//   setpointStr.erase( std::remove(setpointStr.begin(), setpointStr.end(), '\r'), setpointStr.end() );
//   setpointStr.erase( std::remove(setpointStr.begin(), setpointStr.end(), '\n'), setpointStr.end() );
//   
//   setpointStr.erase(0,1);
//   int wordlen;
//   int node;
//   int chained;
//   int type;
//   uint16_t value;
// //     std::cout<<s<<std::endl;
//   std::stringstream(setpointStr.substr(0,2)) >> wordlen;
// //     std::cout<<"wordlen: "<<wordlen<<std::endl;
//   setpointStr.erase(0,2);
// //       std::cout<<s<<std::endl;
//   std::stringstream(setpointStr.substr(0,2)) >> node;
// //       std::cout<<"node: "<<node<<std::endl;
//   setpointStr.erase(0,2);
// //       std::cout<<s<<std::endl;
//   setpointStr.erase(0,2); //command "02"
//   setpointStr.erase(0,2); //process 
//   std::stringstream(setpointStr.substr(0,2)) >> type;
//   setpointStr.erase(0,2); //parameter
//       std::cout<<s<<std::endl;
//   std::cout<<type<<std::endl;
  
  
  
//   std::cout << "Bronkhorst answered: " << debugs << std::endl;
  std::cout << "I asked: " << m1.toString() << std::endl;
  std::cout << "I asked: " << m2.toString() << std::endl;
  std::cout << "I understood: Length" << ": "<< a1.getlength() << " Node:" << a1.getNode() << " Type:" << a1.getType() << " valueType:" << a1.getParameterType() << " value:" << a1.getValueString() << std::endl;
  std::cout << "I understood: Length" << ": "<< a2.getlength() << " Node:" << a2.getNode() << " Type:" << a2.getType() << " valueType:" << a2.getParameterType() << " value:" << a2.getValueString() << std::endl;
  float capacity;
  double setpoint;
  try
  {
    if(!a1.isStatusMessage())
    {
      int iSetpoint;
      std::stringstream(a1.getValueString()) >>  iSetpoint;
      setpoint = (iSetpoint/32767)*capacity;
      std::stringstream(a2.getValueString()) >> capacity;
      std::cout << "Setpoint is: " << iSetpoint << " of 32767 which calculates to " << setpoint << " of " << capacity << std::endl;
    }
    else
      std::cout << "Could not cast string to value! Setting value to zero." << std::endl;
  }
  catch(std::exception& e)
  {
    std::cout << "Could not cast string to value! Setting value to zero." << std::endl;
    setpoint=0.0;
    capacity=0.0;
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
  returnvalue.setvalue(setpoint);
  returnvalue.setunit("sccm");
  return returnvalue;
  
}

std::string bronkhorst::set_flow(float value)
{
  
  if(value == std::numeric_limits<float>::infinity())return "Bad flow request.";
  union { float fValue ; std::uint32_t iValue ; };
  fValue = value ;
  
  static_assert( std::numeric_limits<float>::is_iec559,
                   "For the bronkhorst communication-classes to work properly your computer must support IEEE standard-conformant float values!!" ) ;
  
    std::ostringstream request;
    request << std::hex << std::uppercase << std::setfill('0') << std::setw (8) << iValue;
    std::string s = request.str();
    std::cout << "I told the bronkhorst to set flow to " << value << " (" << s << ")" << std::endl;
          
  return this->inputOutput(":0803022143"+s+"\r\n");
  
}

void bronkhorst::initImplementation()
{
  this->input(":050301000A52\r\n:050302010412\r\n:070304006000600F\r\n");
}

bool bronkhorst::isConnectedImplementation()
{
  return true;
}

void bronkhorst::shutdownImplementation()
{
}
