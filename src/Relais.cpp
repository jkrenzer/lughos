// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "Relais.hpp"
#include <math.h>  


Relais::Relais()
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

template <class T> void Relais::setDefaultImpl(T& connection)
{
}



Relais::~Relais(void)
{

}

template <> void Relais::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);
    connection.end_of_line_char_='$';
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);

}

std::string Relais::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
//       requestString+=std::string("\r\n");

    return requestString;
  
}



  void Relais::set_default()
{
   this->setDefaultImpl(*(this->connection.get()));
}

std::string Relais::read_channels()
{
      std::string responseString="";

  std::string state = this->inputOutput("\x0f");
  int a;
  state>>std::hex>>a;
  for(int i=0; i<8; i++)
  {
    channel_bench[i]=(a&i);
    responseString += std::to_string(channel_bench[i]);
  }
  
 return    responseString;
}

std::string Relais::write_channels(std::to_string channels)
{
    int input_int=0; 
    string::iterator i;
    int counter=0;
    for (i=channels.begin(); i!=channels.end(); i++)
    {
      counter++;
       if (*i!= "0"&&*i!= "1") return "Input error";
       else if (*i= "1") input_int+= exp(2,counter);
    }

    std::ostringstream request;
    request << std::hex << input_int;

    std::string s = request.str();
  this->inputOutput("\xf0"+s);
 
 return    read_channels();
}

std::string Relais::write_channel(int channel, bool onoff)
{
    int input_int=0; 
//     string::iterator i;
//     int counter=0;
    for (int i=1; i<9; i++)
    {
       if (i!=channel) input_int+= exp(2,channel_bench[i]);
       else if (i=channel && onoff == true &&int(onoff)!=channel_bench[i])input_int+= exp(2,i);
       else if (i=channel&&int(onoff)=channel_bench[i]) input_int+= exp(2,channel_bench[i]);
    }

    std::ostringstream request;
    request << std::hex << input_int;

    std::string s = request.str();
    this->inputOutput("\xf0"+s);
 
 return    read_channels();
}

std::string Relais::interpretAnswer(std::string s)
{  
  return s;   
}

void Relais::initImplementation()
{

}
    

void Relais::shutdownImplementation()
{
}

