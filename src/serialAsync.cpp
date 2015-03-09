#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "serialConnections.hpp"
// #include "Dict.hpp"
#include "serialAsync.hpp"
#include "log.hpp"


serialAsync::serialAsync()
{
  if(this->endOfLineRegExpr_== boost::regex("\r"))  std::cout<<"End of line char: "<<"CR"<<std::endl;
  else if(this->endOfLineRegExpr_==boost::regex("\n")) std::cout<<"End of line char: "<<"NL"<<std::endl;
  else std::cout<<"End of line char:"<<this->endOfLineRegExpr_<<std::endl;
}

serialAsync::~serialAsync(void)
{
    stop();
}



int serialAsync::write(std::string query, boost::regex regExpr = boost::regex())
{    
  this->currentQuery= query;
//       start();
      std::ostream request_stream(&request);
      request_stream<<query;
	if(regExpr.empty())
	  regExpr = endOfLineRegExpr_;
	boost::system::error_code ec;
 
	if (port_.get() == NULL || !port_->is_open()) start();
	if (port_.get() == NULL || !port_->is_open()) return 0;

	  boost::asio::async_write(*port_, request,
          boost::bind(&serialAsync::handle_write_request, this, regExpr,
          boost::asio::placeholders::error));
	
	lughos::debugLog(std::string("\"")+query+std::string("\" written to port ")+port_name);
	
	  try 
	  {
	    io_service_->poll();
	  }
	  catch(...)
	  {
	    lughos::debugLog(std::string("I/O-Service exception while polling for port ") + port_name);
	  }


	  if (port_.get() == NULL || !port_->is_open())	lughos::debugLog(port_name + std::string(" is closed despite writing?!"));
	  if (io_service_->io_service::stopped()) lughos::debugLog(std::string("I/O-Service was stopped after or during writing on port ") + port_name);

  
  return 1;
}


void serialAsync::handle_write_request(boost::regex& regExpr, const boost::system::error_code& err)
  {

    if (!err)
    {
      // Read the response status line.
      boost::asio::async_read_until(*port_, response, regExpr,
          boost::bind(&serialAsync::handle_read_content, this, regExpr,
            boost::asio::placeholders::error));
      lughos::debugLog(std::string("Reading until \"")+regExpr.str()+std::string("\" from ") + port_name);
      
    }
    else
    {
      lughos::debugLog(std::string("Error while writing twoway to ") + port_name);
    }
  }
  
  

void serialAsync::handle_read_content(boost::regex& regExpr, const boost::system::error_code& err)
  {
//   	this->timeoutTimer.cancel();
    if (!err)
    {
      // Write all of the data that has been read so far.
        response_string_stream.str(std::string(""));
	response_string_stream<< &response;
	lughos::debugLog(std::string("Read \"") + response_string_stream.str() + std::string("\" from ")+ port_name);
	this->notifyWaitingClient();
	this->currentQuery.clear();
// 	std::cout<<response_string_stream<<std::endl;

    }
    else if (err != boost::asio::error::eof)
    {
      lughos::debugLog(std::string("Error while reading content from ") + port_name);
    }

  } 
  
  void serialAsync::wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error)
  {
    //std::cout << " Calling wait-handler.";
    if (error)
    {
      // Data was read and this timeout was canceled
      lughos::debugLog(std::string("Timeout cancelled for ") + port_name + std::string(" because data was read sucessfully."));
      return;
    }
    //std::cout << " Timed out.";
    try
    {
      this->port_->cancel();
      lughos::debugLog(std::string("Timed out while waiting for answer on ") + port_name);
    }
    catch(...)
    {
      lughos::debugLog(std::string("Exception while timeout and cancelling operation on ") + port_name);
    }
  } 

void serialAsync::abort()
{
  try
  {
    this->port_->cancel();
    lughos::debugLog(std::string("Requested abort on ") + port_name);
  }
  catch(...)
  {
    lughos::debugLog(std::string("Error while trying to perform requested abort on ") + port_name);
  }
}


  
  
