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
    connection.end_of_line_char_='\n';//unconfirmed
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
}


RFG::~RFG(void)
{

}

std::string RFG::composeRequest(std::string query)
{

    std::string requestString="";
    requestString+=query;
    requestString+=std::string("\r");

    return requestString;
  
}



std::string RFG::interpretAnswer(std::string s)
{ 
  s.erase( std::remove(s.begin(), s.end(), '\r'), s.end() );
  s.erase( std::remove(s.begin(), s.end(), '\n'), s.end() );
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


int RFG::set_power_max(int i)
{
  if(power_min>i) return 0;
  std::stringstream stream;
  stream << std::hex << i;
  std::string request= stream.str();
  stream << this->inputOutput("U"+std::string(request)+"\r").erase(0,1);
  int value;
  stream >> std::hex >> value;
 return value; 
}


int RFG::set_power_min(int i)
{
  if(power_max<i) return 0;
  std::stringstream stream;
  stream << std::hex << i;
  std::string request= stream.str();
  stream << this->inputOutput("M"+std::string(request)+"\r").erase(0,1);
  int value;
  stream >> std::hex >> value;
 return value; 
}

int RFG::set_current_lim(int i)
{
  std::stringstream stream;
  stream << std::hex << i;
  std::string request= stream.str();
  stream << this->inputOutput("I"+std::string(request)+"\r").erase(0,1);
  int value;
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
  int value;
  std::stringstream stream;
  std::string s = this->inputOutput("\x32");
  boost::posix_time::ptime now= boost::posix_time::second_clock::local_time();

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
this->input("\r""\x0""AF");
this->mode=true;
controler =0;
}
    

void RFG::shutdownImplementation()
{
}

