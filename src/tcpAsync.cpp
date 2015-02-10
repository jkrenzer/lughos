#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "tcpConnections.hpp"
#include "Dict.hpp"
#include "tcpAsync.hpp"


tcpAsync::tcpAsync(boost::shared_ptr<boost::asio::io_service> io_service)  : Connection<tcpContext>(io_service)
{
  start();
  this->end_of_line_char_ = '\n';
}

tcpAsync::~tcpAsync(void)
{
  stop();
}

bool tcpAsync::connect()
{
  if (!this->connected)
    resolver->async_resolve(*this->query, boost::bind(&tcpAsync::handle_resolve, this,
          boost::asio::placeholders::error, boost::asio::placeholders::iterator));
}

bool tcpAsync::disconnect()
{
}

int tcpAsync::write(std::string query, boost::regex regExpr)
{ 
  if(regExpr.empty())
	  regExpr = boost::regex(std::string(1, end_of_line_char_));
  if(!this->connected)
  {
    this->connect();
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000)); //Wait half a second for the connection
    if(!this->connected)
      std::cout << " BOOOYA!!!!!!!!!!!!!!!!!!" << std::endl;
      throw std::string("Booya!!"); //Still not connected, we abort!
  }
  std::ostream request_stream(&request);
  request_stream<<query;
  if (!writeonly)
  {
    // The connection was successful. Send the request.
    boost::asio::async_write(*socket, request,
	boost::bind(&tcpAsync::handle_write_request, this, regExpr,
	  boost::asio::placeholders::error));
  }
  else
  {
    // The connection was successful. Send the request.
    boost::asio::async_write(*socket, request,
	boost::bind(&tcpAsync::handle_write_only_request, this,
	  boost::asio::placeholders::error));
  }
  return 0;
}

int tcpAsync::write_only(std::string query)
{
    writeonly=true;
    this->write(query);
    writeonly = false;

  return 1;
}


//-------
void tcpAsync::handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{

  if (!err)
  {

    tcp::endpoint endpoint = *endpoint_iterator;
    socket->async_connect(endpoint,
	boost::bind(&tcpAsync::handle_connect, this,
	  boost::asio::placeholders::error, ++endpoint_iterator));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
  }

}

void tcpAsync::handle_connect(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{ 
  if(!err)
  {
    this->connected = true;
    return;
  }
  if (endpoint_iterator != tcp::resolver::iterator())
  {
    // The connection failed. Try the next endpoint in the list.
    socket.reset(new boost::asio::ip::tcp::socket(*this->io_service_));
    tcp::endpoint endpoint = *endpoint_iterator;
    socket->async_connect(endpoint,
	boost::bind(&tcpAsync::handle_connect, this,
	  boost::asio::placeholders::error, ++endpoint_iterator));
  }
  else
  {
    std::cout << "Error: " << err.message() << "\n";
    return;
  }
}

void tcpAsync::handle_write_request(boost::regex regExpr, const boost::system::error_code& err)
{

  if (!err)
  {
    // Read the response status line.
    boost::asio::async_read_until(*socket, response, regExpr,
	boost::bind(&tcpAsync::handle_read_content, this,
	  boost::asio::placeholders::error));
  }
else
  {
    std::cout << "Error: " << err.message() << "\n";
  }
}
  
void tcpAsync::handle_write_only_request(const boost::system::error_code& err)
{

  if (!err)
  {   
  }
  else
  {
    std::cout << "Error Async write only: " << err.message() << "\n";
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
//  void tcpAsync::handle_read_headers(const boost::system::error_code& err)
//   {
// 
//     if (!err)
//     {
//       Connection< tcpContext >::handle_read_headers_process();
// 
//       // Start reading remaining data until EOF.
//       boost::asio::async_read(*socket, response,
//           boost::asio::transfer_at_least(1),
//           boost::bind(&tcpAsync::handle_read_content, this,
//             boost::asio::placeholders::error));
// 
//       
//     }
//     else
//     {
//       std::cout << "Error: " << err << "\n";
//     }
//   }

void tcpAsync::handle_read_content(const boost::system::error_code& err)
  {

    if (!err)
    {
//       response_string_stream.str(std::string(""));
      // Write all of the data that has been read so far.
	response_string_stream<< &response;
      // Continue reading remaining data until EOF.
      boost::asio::async_read(*socket, response,
          boost::asio::transfer_at_least(1),
          boost::bind(&tcpAsync::handle_read_content, this,
            boost::asio::placeholders::error));
      
    }
    else if (err != boost::asio::error::eof || err != boost::asio::error::connection_reset )
    {
      std::cout << "Error: " << err << "\n";
    }
	response_string_stream<< &response;
      	this->notifyWaitingClient();
  }   
  
  void tcpAsync::abort()
{

}

