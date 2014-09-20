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
  
    boost::system::error_code error = boost::asio::error::host_not_found;
     
    connection<tcpContext>::compose_request_stream(buf);
  


    boost::asio::write(socket, request_);

    // Read the response status line.
    boost::asio::read_until(socket, response_, "\r\n");

      connection<tcpContext>::handle_read_check_response(error);
    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response_, "\r\n\r\n");

    connection<tcpContext>::handle_read_headers_process();

    // Read until EOF, writing data to output as we go.
    while (boost::asio::read(socket, response_,
          boost::asio::transfer_at_least(1), error))
      std::cout << &response_;
    
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
  
    return 1;
}



int connection<tcpContext>::write_async(const std::string &buf)
{    
    connection<tcpContext>::compose_request_stream(buf);
    resolver.async_resolve(*this->query,
        boost::bind(&connection<tcpContext>::handle_resolve, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::iterator));
    
           io_service_.run();     
  return 1;
}

void connection<tcpContext>::compose_request_stream(const std::string &buf)
{
    httpDict* http =new httpDict();
    http->compose_request(this->server, buf, &request_);
    if (http) delete http;
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
  
   void connection<tcpContext>::handle_read_check_response(const boost::system::error_code& err)
  {
    httpDict* http =new httpDict();
    http->check_response(&response_);
    if (http) delete http;
   
  }

  void connection<tcpContext>::handle_read_status_line(const boost::system::error_code& err)
  {
    if (!err)
    {
      connection<tcpContext>::handle_read_check_response(err);
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
      connection<tcpContext>::handle_read_headers_process();

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

void connection<tcpContext>::handle_read_headers_process()
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