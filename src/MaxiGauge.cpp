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