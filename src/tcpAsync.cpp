#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "tcpConnections.hpp"
#include "Dict.hpp"
#include "tcpAsync.hpp"
#include "log.hpp"


tcpAsync::tcpAsync()
{
  this->endOfLineRegExpr_ = boost::regex("\n");
}

tcpAsync::~tcpAsync(void)
{
}

void tcpAsync::connect(boost::function<void(void)> callback)
{
  this->socket.reset(new tcp::socket(*io_service_));
  if (!this->endpoint->address().is_unspecified())
  {
    socket->async_connect(*this->endpoint,
	boost::bind(&tcpAsync::handle_connect, this, callback,
	  boost::asio::placeholders::error, tcp::resolver::iterator()));
    lughos::debugLog(std::string("Connecting to server ")+server_name);
  }
  else
  {
    resolver->async_resolve(*this->query, boost::bind(&tcpAsync::handle_resolve, this, callback,
          boost::asio::placeholders::error, boost::asio::placeholders::iterator));
    lughos::debugLog(std::string("Trying to connect to ") + server_name);
  }
}

void tcpAsync::disconnect()
{
  this->abort();
}

int tcpAsync::write(std::string query, boost::regex regExpr)
{ 
  this->currentQuery = query;
  if(regExpr.empty())
	  regExpr = endOfLineRegExpr_;
  if(!this->connected)
  {
    this->connect(boost::bind(&tcpAsync::write,this,query,regExpr));
    return -1;
  }
  
  std::ostream request_stream(&request);
  request_stream<<query;
    // The connection was successful. Send the request.
    boost::asio::async_write(*socket, request,
	boost::bind(&tcpAsync::handle_write_request, this, regExpr,
	  boost::asio::placeholders::error));
    lughos::debugLog(std::string("\"")+query+std::string("\" written to ")+server_name+std::string(":")+port_name);

  try 
  {
    io_service_->poll();
  }
  catch(...)
  {
    lughos::debugLog(std::string("I/O-Service exception while polling for ") +server_name+std::string(":")+port_name);
  }


  if (socket.get() == NULL || !socket->is_open())	lughos::debugLog(server_name + std::string("'s socket is closed despite writing?!"));
  if (io_service_->io_service::stopped()) lughos::debugLog(std::string("I/O-Service was stopped after or during writing on server ") + server_name);
  return 0;
}


//-------
void tcpAsync::handle_resolve(boost::function<void()> callback, const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{

  if (!err)
  {

    *this->endpoint = *endpoint_iterator;
    socket->async_connect(*this->endpoint,
	boost::bind(&tcpAsync::handle_connect, this, callback,
	  boost::asio::placeholders::error, ++endpoint_iterator));
    lughos::debugLog(std::string("Resolved address of server ")+server_name);
  }
  else
  {
    lughos::debugLog(std::string("Unable to resolve address of server ")+server_name+std::string(". Got error: ")+err.message());
  }

}

void tcpAsync::handle_connect(boost::function<void (void)> callback, const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{ 
  if(!err)
  {
    this->connected = true;
    lughos::debugLog(std::string("Connected successfully to ")+server_name);
    if(callback)
      callback();
    return;
  }
  if (endpoint_iterator != tcp::resolver::iterator())
  {
    // The connection failed. Try the next endpoint in the list.
    socket.reset(new boost::asio::ip::tcp::socket(*this->io_service_));
    *this->endpoint = *endpoint_iterator;
    socket->async_connect(*this->endpoint,
	boost::bind(&tcpAsync::handle_connect, this, callback,
	  boost::asio::placeholders::error, ++endpoint_iterator));
    lughos::debugLog(std::string("Connection failed, trying next possible resolve of ")+server_name);
  }
  else
  {
    lughos::debugLog(std::string("Unable to connect to server ")+server_name+std::string(". Got error: ")+err.message());
    this->endpoint.reset(new tcp::endpoint);
    return;
  }
}

void tcpAsync::handle_write_request(boost::regex& regExpr, const boost::system::error_code& err)
{

  if (!err)
  {
    // Read the response status line.
    boost::asio::async_read_until(*socket, response, regExpr,
	boost::bind(&tcpAsync::handle_read_content, this, regExpr,
	  boost::asio::placeholders::error));
    lughos::debugLog(std::string("Reading until \"")+regExpr.str()+std::string("\" from ") + server_name);
  }
else
  {
    lughos::debugLog(std::string("Unable to write to server ")+server_name+std::string(". Got error: ")+err.message());
    this->connected = false;
  }
}
  
//   void tcpAsync::handle_read_status_line(const boost::system::error_code& err)
//   {
// 
//     if (!err)
//     {
// 
//       tcpAsync::handle_read_check_response(err);
//       // Read the response headers, which are terminated by a blank line.
//       boost::asio::async_read_until(*socket, response, "\r\n\r\n",
//           boost::bind(&tcpAsync::handle_read_headers, this,
//             boost::asio::placeholders::error));
// 
//       
//     }
//     else
//     {
//       std::cout << "Error: " << err << "\n";
//     }
//   }
// 

 void tcpAsync::handle_read_rest(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Start reading remaining data until EOF.
      boost::asio::async_read(*socket, response,
          boost::asio::transfer_at_least(1),
          boost::bind(&tcpAsync::handle_read_rest, this,
            boost::asio::placeholders::error));

      
    }
    else if (err == boost::asio::error::eof)
    {
        this->notifyWaitingClient(); 
        return;
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

void tcpAsync::handle_read_content(boost::regex& regExpr, const boost::system::error_code& err)
  {

    if (!err)
    {
        this->handle_read_rest(err);
        response_string_stream.str(std::string(""));
	response_string_stream<< &response;
	lughos::debugLog(std::string("Read \"") + response_string_stream.str() + std::string("\" from ") + server_name);
	this->currentQuery.clear();
    }
    else if (err == boost::asio::error::connection_aborted || err == boost::asio::error::not_connected || err != boost::asio::error::eof || err != boost::asio::error::connection_reset)
    {
      this->connected = false;
      this->write(this->currentQuery,regExpr);
    }
    else
    {
      lughos::debugLog(std::string("Unable to read from server ")+server_name+std::string(". Got error: ")+err.message());
      
    }
  }   
  
  void tcpAsync::abort()
{
 try
  {
    this->socket->cancel();
    lughos::debugLog(std::string("Requested abort on ") + server_name + std::string(":") + port_name);
  }
  catch(...)
  {
    lughos::debugLog(std::string("Error while trying to perform requested abort on ") + server_name + std::string(":") + port_name);
  }
  this->socket.reset(new tcp::socket(*io_service_));
}

