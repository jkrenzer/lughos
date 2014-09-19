// #include "StdAfx.h"

// #include <Setupapi.h>
// #pragma comment(lib, "Setupapi.lib")

#include "tcpConnections.hpp"


connection<tcpContext>::connection(void) :io_service_(), socket(io_service_), socket_async(io_service_), resolver(io_service_)
{
  this->query=NULL;
}

connection<tcpContext>::~connection(void)
{
  if (this->query) delete this->query;
	stop();
}


bool connection<tcpContext>::start(const char *server_name)
{
    this->server=server_name;
    this->query= new tcp::resolver::query(server_name, "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(*this->query);
    tcp::resolver::iterator end;

    // Try each endpoint until we successfully establish a connection.

    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
      socket.close();
      socket.connect(*endpoint_iterator++, error);
    }
    if (error)
      throw boost::system::system_error(error);

	return true;

}



void connection<tcpContext>::reset()
{
}


void connection<tcpContext>::stop()
{
  
}

int connection<tcpContext>::write(const std::string &buf)
{
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
  

  
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    boost::system::error_code error = boost::asio::error::host_not_found;
//     std::string port_name = "192.168.178.40";
    std::string host_path = "/scpi_response.html?cmd=";
    
    
        

    request_stream << "GET " << host_path.c_str()<<buf << " HTTP/1.0\r\n";
    request_stream << "Host: " << this->server<< "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(socket, request);

    // Read the response status line.
    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n");

    // Check that response is OK.
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
      std::cout << "Invalid response\n";
      return 1;
    }
    if (status_code != 200)
    {
      std::cout << "Response returned with status code " << status_code << "\n";
      return 1;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
      std::cout << header << "\n";
    std::cout << "\n";

    // Write whatever content we already have to output.
    if (response.size() > 0)
      std::cout << &response;

    // Read until EOF, writing data to output as we go.
    while (boost::asio::read(socket, response,
          boost::asio::transfer_at_least(1), error))
      std::cout << &response;
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
  
    return 1;
}



int connection<tcpContext>::write_async(const std::string &buf)
{    
  
//     tcp::resolver::query query("192.168.178.40", "http");
    std::ostream request_stream(&request_);
    
    
    std::string port_name = this->server;
    std::string host_path = "/scpi_response.html?cmd=";
    
    request_stream << "GET " << host_path.c_str()<<buf.c_str()<< " HTTP/1.0\r\n";
    request_stream << "Host: " << this->server<< "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.

    resolver.async_resolve(*this->query,
        boost::bind(&connection<tcpContext>::handle_resolve, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::iterator));
    
     std::cout << "GET " << host_path.c_str()<<buf.c_str()<< " HTTP/1.0\r\n";
         

      io_service_.run();     
  return 1;
}



void connection<tcpContext>::set_port()
{

}


//-------
void connection<tcpContext>::handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {

      tcp::endpoint endpoint = *endpoint_iterator;
      socket_async.async_connect(endpoint,
          boost::bind(&connection<tcpContext>::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

 void connection<tcpContext>::handle_connect(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {
    if (!err)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_async, request_,
          boost::bind(&connection<tcpContext>::handle_write_request, this,
            boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
      // The connection failed. Try the next endpoint in the list.
      socket_async.close();
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_async.async_connect(endpoint,
          boost::bind(&connection<tcpContext>::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

void connection<tcpContext>::handle_write_request(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Read the response status line.
      boost::asio::async_read_until(socket_async, response_, "\r\n",
          boost::bind(&connection<tcpContext>::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }

  void connection<tcpContext>::handle_read_status_line(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Check that response is OK.
      std::istream response_stream(&response_);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        std::cout << "Invalid response\n";
        return;
      }
      if (status_code != 200)
      {
        std::cout << "Response returned with status code ";
        std::cout << status_code << "\n";
        return;
      }

      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_async, response_, "\r\n\r\n",
          boost::bind(&connection<tcpContext>::handle_read_headers, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

 void connection<tcpContext>::handle_read_headers(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Process the response headers.
      std::istream response_stream(&response_);
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
        std::cout << header << "\n";
      std::cout << "\n";

      // Write whatever content we already have to output.
      if (response_.size() > 0)
        std::cout << &response_;

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_async, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&connection<tcpContext>::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

void connection<tcpContext>::handle_read_content(const boost::system::error_code& err)
  {
    if (!err)
    {
      // Write all of the data that has been read so far.
      std::cout << &response_;

      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_async, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&connection<tcpContext>::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error: " << err << "\n";
    }
  }


    