#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "serialConnections.hpp"
// #include "Dict.hpp"
#include "serialAsync.hpp"


serialAsync::serialAsync(boost::shared_ptr<boost::asio::io_service> io_service)  : Connection<serialContext>(io_service)
{
     start(); 
}

serialAsync::~serialAsync(void)
{
    stop();
}



int serialAsync::write(std::string query)
{    
  
//       start();
      std::ostream request_stream(&request);
      request_stream<<query;
    
// 	const int size=request_stream;
	boost::system::error_code ec;
// 	if (!port_) return -1;
// 	if (size == 0) return 0;
// 	 io_service_->post(async_read_some_);
// 	io_service_->run();

   	 async_read_some_();
  
  return 0;
}


void serialAsync::async_read_some_()
{
  
  
//   std::cout<<"bist du versackt?"<<std::endl;
 
	if (port_.get() == NULL || !port_->is_open()) start();
	if (port_.get() == NULL || !port_->is_open()) return;
// 	  boost::asio::write(*port_, request);
	  boost::asio::async_write(*port_, request,
          boost::bind(&serialAsync::handle_write_request, this,
          boost::asio::placeholders::error));
// 	
// 	  std::cout<<port_name<<std::endl;
	  io_service_->poll();
	  this->timeoutTimer.expires_from_now(boost::posix_time::millisec(1000));
          this->timeoutTimer.wait();

	  if (port_.get() == NULL || !port_->is_open())	std::cout<<"port is somehow closed again"<<std::endl;
	  if(io_service_->io_service::stopped())std::cout<<"Io service gestoppt"<<std::endl;
	  
// if(	io_service_->)std::cout<<"io_service has_service"<<std::endl;
	
// if (port_.get() == NULL || !port_->is_open())std::cout<<"wer hat dich denn zu gemacht?"<<std::endl;
}


void serialAsync::handle_write_request(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Read the response status line.
      boost::asio::async_read_until(*port_, response, end_of_line_char_,
          boost::bind(&serialAsync::handle_read_content, this,
            boost::asio::placeholders::error));
    
      
    }
    else
    {
      std::cout << "Error Async: " << err.message() << "\n";
    }
  }
  
  

void serialAsync::handle_read_content(const boost::system::error_code& err)
  {
  	this->timeoutTimer.cancel();
    if (!err)
    {
      // Write all of the data that has been read so far.
	response_string_stream<< &response;
// 	std::cout<<response_string_stream<<std::endl;

    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error ReadContent: " << err << "\n";
    }

  } 




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

  