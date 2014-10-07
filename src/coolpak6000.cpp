// #include "StdAfx.h"

#include <ostream>
// #pragma comment(lib, "Setupapi.lib")
#include "serialSync.hpp"
#include "serialAsync.hpp"
#include "coolpak6000.hpp"


coolpak6000::coolpak6000(boost::asio::io_service * io_service):serialSync(io_service), serialAsync(io_service), connection< serialContext >(io_service)
{
 
set_default();

}



coolpak6000::~coolpak6000(void)
{

}

void coolpak6000::compose_request(const std::string &buf)
{
//         std::cout<<"composed_"<<std::endl;
//   boost::asio::streambuf buff;
//   std::ostream request_stream(&buff);

    std::ostream request_stream(&request_);

    request_stream<<"\x02" <<buf.c_str()<< "\r";
//       std::cout<<"composed_"<<&request_<<std::endl;
    return;
  
}

   std::string coolpak6000::inputoutput(const std::string input, const int async)
{
    if (async==0)write(input);
    else if (async==1)write_async(input);
    else write(input);
    return read();
}

   void coolpak6000::set_default()
{
    this->baud_rate=boost::asio::serial_port_base::baud_rate(4800);
    this->flow_control=boost::asio::serial_port_base::flow_control::none;
    this->character_size=boost::asio::serial_port_base::character_size(8);
    this->end_of_line_char_='\r';
    this->parity=boost::asio::serial_port_base::parity::none;
    this->stop_bits=boost::asio::serial_port_base::stop_bits::one;
}

std::string coolpak6000::read()
{
        std::string s = response_string_stream.str();

	response_string_stream.str("");
	
	static const boost::regex e("^\\D*(\\d*)\\D$");
	 boost::cmatch res;
	 boost::regex_search(s.c_str(), res, e);
    return res[1];  

}

   bool coolpak6000::compressor_on()
{
  std::string response=this->inputoutput("SYS1");
  if(response.c_str()=="SYS1")return true;
  else if(response.c_str()=="SYS2")return false;
  else return false; 
  
}

   bool coolpak6000::compressor_off()
{
  std::string response=this->inputoutput("SYS0");
  if(response.c_str()=="SYS0")return true;
  else if(response.c_str()=="SYS2")return false;
  else return false; 
  
}

   std::string coolpak6000::get_data()
{
  return this->inputoutput("DAT");
  
}

   std::string coolpak6000::get_error_list()
{
  return this->inputoutput("ERR");
  
}

   bool coolpak6000::coolhead_on(int head)
{
 
  std::string request("SC" +std::to_string(head)+std::to_string(1));
   
  std::string response=this->inputoutput(request);
  if(response.c_str()==request.c_str())return true;
  else return false; 
  
}

   bool coolpak6000::coolhead_off(int head)
{
  std::string request("SC" +std::to_string(head)+std::to_string(0));
   
  std::string response=this->inputoutput(request);
  if(response.c_str()==request.c_str())return true;
  else return false; 
  
}

