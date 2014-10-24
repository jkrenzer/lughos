// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialConnections.hpp"
#include "MaxiGauge.hpp"


MaxiGauge::MaxiGauge()
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

template <class T> void MaxiGauge::setDefaultImpl(T& connection)
{
}

template <> void MaxiGauge::setDefaultImpl< Connection<serialContext> > (Connection<serialContext>& connection)
{
  
    connection.baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    connection.flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
    connection.character_size=boost::asio::serial_port_base::character_size(8);
    connection.end_of_line_char_='\x0A';
    connection.parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
    connection.stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
}

MaxiGauge::~MaxiGauge(void)
{

}

std::string MaxiGauge::composeRequest(std::string query)
{
    std::string requestString="";
    requestString+=query;
    requestString+=std::string("\r");

    return requestString;
  
}

  void MaxiGauge::set_default()
{
   this->setDefaultImpl(*(this->connection.get()));
}

std::string MaxiGauge::interpretAnswer(std::string s)
{   
    if(s=="\x15""\r\n") return "NAK";
    else if(s=="\x06""\r\n") return this->inputOutput("\x05");
    else
    {	
	 static const boost::regex e("^(\\d*)\\D\\D$");
	 boost::cmatch res;
	 boost::regex_search(s.c_str(), res, e);
	 return res[1];   
    }

}


   bool MaxiGauge::sensor_on(int sensor)
{
  int check_bench[6];
  static const boost::regex e("(\\d)");
  boost::cmatch res;
  boost::regex_search(  this->inputOutput("SEN,0,0,0,0,0,0").c_str(), res, e);
  for(int i=0;i<6;i++) sensor_bench[i]=boost::lexical_cast< int >(res[i+1]);
	
 	

  std::string request= "SEN,";
  for(int i=0;i<6;i++){
      if(i!=sensor)
      {
	  request.append(std::to_string(0)); 
	
      }
      else request.append(std::to_string(2));
  }
   
  boost::regex_search(  this->inputOutput(request).c_str(), res, e);
  for(int i=0;i<6;i++) check_bench[i]=boost::lexical_cast< int >(res[i+1]);
  
  for(int i=0;i<6;i++){
      if(i!=sensor&&check_bench[i]==sensor_bench[i])
      {
      }
       if(i==sensor&&check_bench[i]==2)
      {
      }
      else return false;
  }
      for(int i=0;i<6;i++) sensor_bench[i]=check_bench[i];
  return true;
  
}

bool MaxiGauge::sensor_off(int sensor)
{
  std::array<int, 6> check_bench;
  static const boost::regex e("(\\d)");
  boost::cmatch res;
  boost::regex_search(  this->inputOutput("SEN,0,0,0,0,0,0").c_str(), res, e);
  for(int i=0;i<6;i++) sensor_bench[i]=boost::lexical_cast< int >(res[i+1]);
	
 	

  std::string request= "SEN,";
  for(int i=0;i<6;i++){
      if(i!=sensor)
      {
	  request.append(std::to_string(0)); 
	
      }
      else request.append(std::to_string(1));
  }
   
  boost::regex_search(  this->inputOutput(request).c_str(), res, e);
  for(int i=0;i<6;i++) check_bench[i]=boost::lexical_cast< int >(res[i+1]);
  
  for(int i=0;i<6;i++){
      if(i!=sensor&&check_bench[i]==sensor_bench[i])
      {
      }
       if(i==sensor&&check_bench[i]==1)
      {
      }
      else return false;
  }
  
    for(int i=0;i<6;i++) sensor_bench[i]=check_bench[i];
  return true;
  
}

  std::string MaxiGauge::get_status(int sensor)
{
  std::string request= "PR";
  request.append(std::to_string(sensor)); 
 
  return this->inputOutput(request);
  
}

void MaxiGauge::initImplementation()
{
}
    

void MaxiGauge::shutdownImplementation()
{
}
