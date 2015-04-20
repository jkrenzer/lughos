// #include "StdAfx.h"

#include <sstream>
// #pragma comment(lib, "Setupapi.lib")
#include "Relais.hpp"
#include <math.h>  


Relais::Relais() : relais("relais"), sensors("sensors")
{
  this->relais.getter(boost::bind(&Relais::get_relais,this));
  this->relais.setter(boost::bind(&Relais::set_relais,this,_1));
  this->sensors.getter(boost::bind(&Relais::get_sensors,this));
}

void Relais::memberDeclaration()
{
  this->addMember(this->relais);
  this->addMember(this->sensors);
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

Relais::~Relais(void)
{

}

RelaisConnection::RelaisConnection()
{
  
    this->baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    this->flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->endOfLineRegExpr_= boost::regex("\\$");
    this->parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    this->stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);

}

std::string Relais::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
//       requestString+=std::string("\r\n");

    return requestString;
  
}

measuredValue< std::bitset<8> > Relais::get_relais()
{
  measuredValue< std::bitset<8> > tmp;
  tmp.setValue(this->get_state_of(Module::RelaisModule));
  return tmp;
}

measuredValue< std::bitset<8> > Relais::get_sensors()
{
  measuredValue< std::bitset<8> > tmp;
  tmp.setValue(this->get_state_of(Module::SensorModule));
  return tmp;
}


std::bitset<8> Relais::get_state_of(Module module)
{
  std::string command;
  command += '\x0f';
  std::string state = this->inputOutput(command);
  if(!state.empty())
  {
    u_int8_t i = static_cast<u_int8_t>(state[module]);
    return std::bitset<8>(i);
  }
  else
    return std::bitset<8>(0); //TODO throw exception if we cannot read properly and set device offline

//   std::cout << "Relais: GET RESPONSE: " << state << " (" << channel_bench.to_string() <<" , sensors: " << sensor_bench.to_string() << ")" << std::endl; 

}

void Relais::set_relais(measuredValue< std::bitset<8> > relais_)
{


    std::string s;
    s.clear();
    s = "\xf0";
    s += static_cast<char>(relais_.getValue().to_ulong());
    std::string  answer=this->inputOutput(s);
    std::bitset<8> answerBS;
    answerBS = (long unsigned int) answer[0];
//     if(!answer.c_str()==static_cast<char>(this->channel_bench.to_ulong()))
    if(answerBS != relais_)
    {
      std::cout << "Relais: SET ERROR: Relais answered"  << answer << " (" << answerBS << ")" << std::endl;
      this->relais.setValue(answerBS);
    }

/*    
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
 return channel_bench.to_string();*/
}

// std::string Relais::write_channel(int channel, bool onoff)
// {
//     int input_int=0; 
// //     string::iterator i;
// //     int counter=0;
// //     for (int i=1; i<9; i++)
// //     {
// //        if (i!=channel) input_int+= std::pow(2,channel_bench[i]);
// //        else if (i==channel && onoff == true &&int(onoff)!=channel_bench[i])input_int+= std::pow(2,i-1);
// //        else if (i==channel&&int(onoff)==channel_bench[i]) input_int+= std::pow(2,channel_bench[i]);
// //     }
//     if (channel < 8 && channel > -1)
//     {
//       this->channel_bench[7-channel] = onoff; //reversed logic!
//       
//       std::string s;
//       s.clear();
//       s = "\xf0";
//       s += static_cast<char>(this->channel_bench.to_ulong());
//       std::string  answer=this->inputOutput(s);
//       if(!answer.c_str()==static_cast<char>(this->channel_bench.to_ulong()))  std::cout << "Relais: SET ERROR: Relais answerd"  <<answer << std::endl;
// 
//     }
//  return    read_channels();
// }

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

bool Relais::isConnectedImplementation()
{
  return true;
}

