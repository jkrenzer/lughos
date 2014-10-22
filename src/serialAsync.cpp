#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "serialConnections.hpp"
// #include "Dict.hpp"
#include "serialAsync.hpp"


serialAsync::serialAsync(boost::asio::io_service* io_service) : Connection<serialContext>(io_service)
{
}

serialAsync::~serialAsync(void)
{

}



int serialAsync::write(std::string query)
{    
  
      start();
      std::ostream request_stream(&request);
      request_stream<<query;
    
	const int size=request.size();
	boost::system::error_code ec;
// 	if (!port_) return -1;
	if (size == 0) return 0;
	 
   	 async_read_some_();
  
  return 0;
}


void serialAsync::async_read_some_()
{
  
//   std::cout<<"bist du versackt?"<<std::endl;
	if (port_.get() == NULL || !port_->is_open()) return;

	  boost::asio::async_write(*port_, request,
          boost::bind(&serialAsync::handle_write_request, this,
          boost::asio::placeholders::error));
	
// 	io_service_->run();
	

}


void serialAsync::handle_write_request(const boost::system::error_code& err)
  {

    if (!err&&port_)
    {
      // Read the response status line.
      boost::asio::async_read_until(*port_, response, end_of_line_char_,
          boost::bind(&serialAsync::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error Async: port or" << err.message() << "\n";
    }
  }
  
  

void serialAsync::handle_read_content(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Write all of the data that has been read so far.
	response_string_stream<< &response;

    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error ReadContent: " << err << "\n";
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

  