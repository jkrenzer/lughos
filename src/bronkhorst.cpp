// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "serialAsync.hpp"
#include "bronkhorst.hpp"


bronkhorst::bronkhorst(void)
{
  set_default();
}



bronkhorst::~bronkhorst(void)
{

}

void bronkhorst::compose_request(const std::string &buf)
{
//         std::cout<<"composed_"<<std::endl;
//   boost::asio::streambuf buff;
//   std::ostream request_stream(&buff);

    std::ostream request_stream(&request_);

    request_stream <<buf.c_str()<< "\r\n";
//       std::cout<<"composed_"<<&request_<<std::endl;
    return;
  
}

   std::string bronkhorst::inputoutput(const std::string input, const int async)
{
    if (async==0)write(input);
    else if (async==1)write_async(input);
    else write(input);
    return read();
}

   void RFG::set_default()
{
    this->baud_rate=boost::asio::serial_port_base::baud_rate(38400);
    this->flow_control=boost::asio::serial_port_base::flow_control::none;
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->end_of_line_char_='\r\n';
    this->parity=boost::asio::serial_port_base::parity::none;
    this->stop_bits=boost::asio::serial_port_base::stop_bits::one;
}

