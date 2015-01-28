// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "serialAsync.hpp"
#include "RFG.hpp"


RFG::RFG()
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
   this->voltage_max =12;
   this->voltage_min =7;
   this->current_max =2;
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
 if(controler!=0)this->input("F"); 
}

void RFG::use_current_controler()
{
 if(controler!=1)this->input("G"); 
}

void RFG::use_power_controler()
{
 if(controler!=2)this->input("H"); 
}

float RFG::getLimitMaxVoltage()
{
  return this->voltage_max;
}

float RFG::getLimitMinVoltage()
{
  return this->voltage_min;
}

float RFG::getLimitMaxCurrent()
{
  return this->current_max;
}

float RFG::set_voltage_max(std::string f)
{
//   if(voltage_min>f) return 0;
  std::stringstream stream;
  stream << std::hex << f;
  std::string request= stream.str();
  stream << this->inputOutput("\x00U"+std::string(f)+"\r").erase(0,1);
  float value;
  stream >> std::hex >> value;
 return value; 
}


float RFG::set_voltage_min(std::string  f)
{
//   if(voltage_max<f) return 0;
  std::stringstream stream;
  stream << std::hex << f;
  std::string request= stream.str();
  stream << this->inputOutput("\x00M"+std::string(f)+"\r").erase(0,1);
  float value;
  stream >> std::hex >> value;
 return value; 
}

float RFG::set_current_lim(std::string  f)
{
  std::stringstream stream;
  stream << std::hex << f;
  std::string request= stream.str();
  stream << this->inputOutput("\x00I"+std::string(f)+"\r").erase(0,1);
  float value;
  stream >> std::hex >> value;
 return value; 
}

int RFG::set_controler_chanel(int i)
{
  if(!(i>=0&&i<=7))return 0;
  std::stringstream stream;
  stream << std::hex << i;
  std::string request= stream.str();
  stream << this->inputOutput("P"+std::string(request)+"\r").erase(0,1);
  int value;
  stream >> std::hex >> value;
 return value; 
}

bool RFG::readout()
{
  int value=0;
  std::stringstream stream;
//   std::cout<<"S: "<<s<<std::endl;
  std::string s = this->inputOutput("\x00\x20\x00");
//   this->inputOutput("\r");
  boost::posix_time::ptime now= boost::posix_time::second_clock::local_time();
  std::cout<<"S: "<<s<<std::endl;
  static const boost::regex e("....(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)(\\d\\d\\d)");
  boost::cmatch res;
  boost::regex_search(s.c_str(), res, e);
  for(int i =0;i<8;i++)
  {
    stream <<res[i+1];
    stream >> value;
    channel_output[i].setvalue(value);
    if (controler==0)channel_output[i].setunit("V");
    if (controler==1)channel_output[i].setunit("I");
    if (controler==2)channel_output[i].setunit("Watt");
    channel_output[i].settimestamp(now);
  }

 return true; 
}

measuredValue RFG::get_channel(int i, bool force)
{
 if(!force &&!channel_output[0].gettimestamp().is_not_a_date_time()&& channel_output[0].gettimestamp()>boost::posix_time::second_clock::local_time()+boost::posix_time::seconds(1))
  {
    return channel_output[i];
  }
  
  this->readout();
 return channel_output[i];
}


void RFG::initImplementation()
{
this->inputOutput(std::string("\x0d")+std::string("AF")+std::string("\r"));//
this->mode=true;
controler =0;
}
    

void RFG::shutdownImplementation()
{
}

