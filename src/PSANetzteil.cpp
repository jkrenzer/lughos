// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "PSANetzteil.hpp"


PSANetzteil::kithleighSerial(void)
{
  set_default();
}



PSANetzteil::~PSANetzteil(void)
{

}

void PSANetzteil::compose_request(const std::string &buf)
{
//         std::cout<<"composed_"<<std::endl;
//   boost::asio::streambuf buff;
//   std::ostream request_stream(&buff);

    std::ostream request_stream(&request_);

    request_stream <<buf.c_str()<< "\r";
//       std::cout<<"composed_"<<&request_<<std::endl;
    return;
  
}

   std::string PSANetzteil::inputoutput(const std::string input, const int async)
{
    if (async==0)write(input);
    else if (async==1)write_async(input);
    else write(input);
    return read();
}

   void PSANetzteil::set_default()
{
    this->baud_rate=boost::asio::serial_port_base::baud_rate(9600);
    this->flow_control=boost::asio::serial_port_base::flow_control::none;
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->end_of_line_char_='\r';
    this->parity=boost::asio::serial_port_base::parity::none;
    this->stop_bits=boost::asio::serial_port_base::stop_bits::one;
}

