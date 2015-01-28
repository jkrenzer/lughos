// #include "StdAfx.h"

#include <sstream>
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

  std::string state = this->inputOutput("\x0f");
  int i = static_cast<int>(state[0]);
  channel_bench = std::bitset<8>(i);

  std::cout << "Relais: GET RESPONSE: " << state << " (" << channel_bench.to_string() << ")" << std::endl; 
  
 return  channel_bench.to_string();
}

std::string Relais::write_channels(std::string channels)
{
//     int input_int=0; 
//     std::string::iterator i;
//     int counter=0;
//     for (i=channels.begin(); i!=channels.end(); i++)
//     {
//       counter++;
//        if (*i!= '0'&&*i!= '1') return "Input error";
//        else if (*i= '1') input_int+= std::pow(2,counter-1);
// //        std::cout<<std::pow(2,counter-1)<<" "<< std::hex<<input_int<<std::endl;
//     }
  
    this->channel_bench = std::bitset<8>(channels);

    std::string s;
    s.clear();
    s = "\xf0";
    s += static_cast<char>(this->channel_bench.to_ulong());
    this->inputOutput(s);
    
    std::bitset<8> helper;
    helper.reset();
    std::string debugString;
    debugString.clear();
    for (int i = 0; i < s.size(); i++)
    {
      helper = static_cast<long unsigned int>(s[i]);
      debugString += helper.to_string();
    }
    
    
 std::cout << "Relais: SET COMMAND: " << s << " = " << debugString << " (" << channels << " = "  << channel_bench.to_string() << " = " << static_cast<char>(this->channel_bench.to_ulong()) << ")" << std::endl;
 //return  read_channels();
 return channel_bench.to_string();
}

std::string Relais::write_channel(int channel, bool onoff)
{
    int input_int=0; 
//     string::iterator i;
//     int counter=0;
//     for (int i=1; i<9; i++)
//     {
//        if (i!=channel) input_int+= std::pow(2,channel_bench[i]);
//        else if (i==channel && onoff == true &&int(onoff)!=channel_bench[i])input_int+= std::pow(2,i-1);
//        else if (i==channel&&int(onoff)==channel_bench[i]) input_int+= std::pow(2,channel_bench[i]);
//     }
    if (channel < 8 || channel > -1)
    {
      this->channel_bench[channel] = onoff;

      std::string s;
      s.clear();
      s = "\xf0";
      s += static_cast<char>(this->channel_bench.to_ulong());
      this->inputOutput(s);
    }
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

