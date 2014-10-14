// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "Relais.hpp"
#include <math.h>  


Relais::Relais(boost::asio::io_service* io_service):serialSync(io_service), serialAsync(io_service), connection< serialContext >(io_service)
{
 
set_default();

}



Relais::~Relais(void)
{

}

void Relais::compose_request(const std::string &buf)
{
//         std::cout<<"composed_"<<std::endl;
//   boost::asio::streambuf buff;
//   std::ostream request_stream(&buff);

    std::ostream request_stream(&request_);

    request_stream<<buf.c_str();
//       std::cout<<"composed_"<<&request_<<std::endl;
    return;
  
}



   std::string Relais::inputoutput(const std::string input, const int async)
{
    if (async==0)write(input);
    if (async==1)write_async(input);
    else write(input);
    return read();
}

  void Relais::set_default()
{
    this->baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    this->flow_control=boost::asio::serial_port_base::flow_control::none;
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->end_of_line_char_='\0';
    this->parity=boost::asio::serial_port_base::parity::none;
    this->stop_bits=boost::asio::serial_port_base::stop_bits::one;
}

std::string Relais::read()
{
  std::string s = response_string_stream.str();
  response_string_stream.str("");
  return s;  

}



std::string Relais::input_read()
{
  int check_bench[8];
  
    std::istringstream buffer(this->inputoutput("\x0f"));

    unsigned long long value;

    buffer >> std::hex >> value;
    for(int i=0;i<8;i=i*i){
      if(value&i)input_bench[i]=1;
      else input_bench[i]=0;
    }
  

    return this->inputoutput("\x0f");
  
}


bool Relais::input_status(int sensor)
{
  this->input_read();

  if(input_bench[sensor])  return true;
  else return false;
  
}


   bool Relais::input_on(int sensor)
{
  input_read();
	
 	

  std::string request= "";
  int request_int= 0;
  for(int i=0;i<1;i++){
      if(i!=sensor)
      {
	request_int+=pow(2,i)*input_bench[i];
	
      }
      else request_int+=pow(2,i);
  }
  std::stringstream stream;
  stream << std::hex << request_int;
  request=stream.str() ;
  
  this->inputoutput("\xf0");
  this->inputoutput(request);
  return input_status(sensor);
  
}

   bool Relais::input_off(int sensor)
{
  input_read();
	
 	

  std::string request= "";
  int request_int= 0;
  for(int i=0;i<1;i++){
      if(i!=sensor)
      {
	request_int+=pow(2,i)*input_bench[i];
	
      }

  }
  std::stringstream stream;
  stream << std::hex << request_int;
  request=stream.str() ;
  
  this->inputoutput("\xf0");
  this->inputoutput(request);
  return input_status(sensor);
  
}

