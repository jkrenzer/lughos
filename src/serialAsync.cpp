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
  this->queryDone = false;
//       start();
      std::ostream request_stream(&request);
      request_stream<<query;

	boost::system::error_code ec;
 
	if (port_.get() == NULL || !port_->is_open()) start();
	if (port_.get() == NULL || !port_->is_open()) return 0;

	  boost::asio::async_write(*port_, request,
          boost::bind(&serialAsync::handle_write_request, this,
          boost::asio::placeholders::error));
	
	  try 
	  {
	    io_service_->poll();
	  }
	  catch(...)
	  {
	    std::cout << "write(): io_service exception!" << std::endl;
	  }


	  if (port_.get() == NULL || !port_->is_open())	std::cout<<"port is somehow closed again"<<std::endl;
	  if(io_service_->io_service::stopped())std::cout<<"Io service gestoppt"<<std::endl;

  
  return 1;
}


int serialAsync::write_only(std::string query)
{
  this->queryDone = false;
//       start();
      std::ostream request_stream(&request);
      request_stream<<query;

	boost::system::error_code ec;
 
	if (port_.get() == NULL || !port_->is_open()) start();
	if (port_.get() == NULL || !port_->is_open()) return 0;

	  boost::asio::async_write(*port_, request,
          boost::bind(&serialAsync::handle_write_only, this,
          boost::asio::placeholders::error));

	   try 
	  {
	    io_service_->poll();
	  }
	  catch(...)
	  {
	    std::cout << "write_only(): io_service exception!" << std::endl;
	  }


  
  return 1;
}


void serialAsync::handle_write_only(const boost::system::error_code& err)
  {

    if (!err)
    {   
    }
    else
    {
      std::cout << "Error Async write only: " << err.message() << "\n";
    }
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
//   	this->timeoutTimer.cancel();
    if (!err)
    {
      // Write all of the data that has been read so far.
        response_string_stream.str(std::string(""));
	response_string_stream<< &response;
	this->notifyWaitingClient();
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
    try
    {
      this->port_->cancel();
    }
    catch(...)
    {
      std::cout << "wait_callback(): exception caught!" << std::endl;
    }
  } 

void serialAsync::abort()
{
  try
  {
    this->port_->cancel();
  }
  catch(...)
  {
    std::cout << "abort(): exception caught!" << std::endl;
  }
}


  
  