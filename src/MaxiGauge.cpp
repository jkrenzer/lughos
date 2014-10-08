// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "MaxiGauge.hpp"


MaxiGauge::MaxiGauge(void)
{
 
set_default();

}



MaxiGauge::~MaxiGauge(void)
{

}

void MaxiGauge::compose_request(const std::string &buf)
{
//         std::cout<<"composed_"<<std::endl;
//   boost::asio::streambuf buff;
//   std::ostream request_stream(&buff);

    std::ostream request_stream(&request_);

    request_stream<<buf.c_str()<< "\r";
//       std::cout<<"composed_"<<&request_<<std::endl;
    return;
  
}

   void MaxiGauge::handle_read_check_response(const boost::system::error_code& err)
{
  std::stringstream check_response_stream;
  check_response_stream<<&response_;
 
  
        std::string s = check_response_stream.str();
// 	std::cout << "keks" << '\n';
// 	response_string_stream.str("");
  
  if(s=="\x06""\r\n"){
    this->write("\x05");}
    else if(s=="\x06""\r\n") std::cout<<"NAK"<<std::endl;
    else  response_string_stream<< s; 
}

   std::string MaxiGauge::inputoutput(const std::string input, const int async)
{
    if (async==0)write(input);
//     if (async==1)write_async(input);
    else write(input);
    return read();
}

  void MaxiGauge::set_default()
{
    this->baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    this->flow_control=boost::asio::serial_port_base::flow_control::none;
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->end_of_line_char_='\x0A';
    this->parity=boost::asio::serial_port_base::parity::none;
    this->stop_bits=boost::asio::serial_port_base::stop_bits::one;
}

std::string MaxiGauge::read()
{
        std::string s = response_string_stream.str();

	response_string_stream.str("");
	
	static const boost::regex e("^(\\d*)\\D\\D$");
	 boost::cmatch res;
	 boost::regex_search(s.c_str(), res, e);
    return res[1];  

}

   bool MaxiGauge::sensor_on(int sensor)
{
  int check_bench[6];
  static const boost::regex e("(\\d)");
  boost::cmatch res;
  boost::regex_search(  this->inputoutput("SEN,0,0,0,0,0,0").c_str(), res, e);
  for(int i=0;i<6;i++) sensor_bench[i]=boost::lexical_cast< int >(res[i+1]);
	
 	

  std::string request= "SEN,";
  for(int i=0;i<6;i++){
      if(i!=sensor)
      {
	  request.append(std::to_string(0)); 
	
      }
      else request.append(std::to_string(2));
  }
   
  boost::regex_search(  this->inputoutput(request).c_str(), res, e);
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
  boost::regex_search(  this->inputoutput("SEN,0,0,0,0,0,0").c_str(), res, e);
  for(int i=0;i<6;i++) sensor_bench[i]=boost::lexical_cast< int >(res[i+1]);
	
 	

  std::string request= "SEN,";
  for(int i=0;i<6;i++){
      if(i!=sensor)
      {
	  request.append(std::to_string(0)); 
	
      }
      else request.append(std::to_string(1));
  }
   
  boost::regex_search(  this->inputoutput(request).c_str(), res, e);
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
 
  return this->inputoutput(request);
  
}
