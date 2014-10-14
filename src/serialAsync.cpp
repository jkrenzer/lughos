#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "serialConnections.hpp"
// #include "Dict.hpp"
#include "serialAsync.hpp"


serialAsync::serialAsync(boost::asio::io_service* io_service) : connection<serialContext>(io_service)
{
}

serialAsync::~serialAsync(void)
{

}



int serialAsync::write_async(const std::string &buf)
{    
  
	compose_request(buf);

	boost::system::error_code ec;
	const int size=buf.size();
// 	if (!port_) return -1;
	if (size == 0) return 0;
	 
   	 async_read_some_();
  
  return 1;
}


void serialAsync::async_read_some_()
{
  
//   std::cout<<"bist du versackt?"<<std::endl;
	if (port_.get() == NULL || !port_->is_open()) return;

	  boost::asio::async_write(*port_, request_,
          boost::bind(&serialAsync::handle_write_request, this,
          boost::asio::placeholders::error));
	
// 	io_service_->run();
	

}


void serialAsync::handle_write_request(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Read the response status line.
      boost::asio::async_read_until(*port_, response_, end_of_line_char_,
          boost::bind(&serialAsync::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }
  
  

void serialAsync::handle_read_content(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Write all of the data that has been read so far.
	response_string_stream<< &response_;

    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error: " << err << "\n";
    }

  } 
//------------------


  void serialAsync::wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error)
  {
    //std::cout << " Calling wait-handler.";
    if (error)
    {
      // Data was read and this timeout was canceled
      return;
    }
    //std::cout << " Timed out.";
    port_.cancel();  // will cause read_callback to fire with an error
  } 

  