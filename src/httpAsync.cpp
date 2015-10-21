/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "tcpConnections.hpp"
#include "Dict.hpp"
#include "httpAsync.hpp"


httpAsync::httpAsync(boost::shared_ptr<boost::asio::io_service> io_service)  : Connection<tcpContext>(io_service)
{
  start(); 
}

httpAsync::~httpAsync(void)
{
  stop();
}



int httpAsync::write(std::string query)
{    

        std::ostream request_stream(&request);
      request_stream<<query;
//  httpAsync::handle_resolve(this, boost::asio::placeholders::error, boost::asio::placeholders::iterator);
 resolver.async_resolve(*this->query_async, boost::bind(&httpAsync::handle_resolve, this,
          boost::asio::placeholders::error, boost::asio::placeholders::iterator));
	  io_service_->poll();
	  
  return 1;
  
}


int httpAsync::write_only(std::string query)
{
    writeonly=true;
    this->write(query);

  return 1;
}


//-------
void httpAsync::handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {

    if (!err)
    {

      tcp::endpoint endpoint = *endpoint_iterator;
      socket.async_connect(endpoint,
          boost::bind(&httpAsync::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));

    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
    

  }

 void httpAsync::handle_connect(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  { 

    if (!err&&!writeonly)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket, request,
          boost::bind(&httpAsync::handle_write_request, this,
            boost::asio::placeholders::error));
    }
    
    else if (!err&&writeonly)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket, request,
          boost::bind(&httpAsync::handle_write_only_request, this,
            boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
      // The connection failed. Try the next endpoint in the list.
      socket.close();
      tcp::endpoint endpoint = *endpoint_iterator;
      socket.async_connect(endpoint,
          boost::bind(&httpAsync::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
    

  }

void httpAsync::handle_write_request(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Read the response status line.
      boost::asio::async_read_until(socket, response, "\r\n",
          boost::bind(&httpAsync::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
  else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }
  
  void httpAsync::handle_write_only_request(const boost::system::error_code& err)
  {

    if (!err)
    {   
    }
    else
    {
      std::cout << "Error Async write only: " << err.message() << "\n";
    }
  }


  void httpAsync::handle_read_status_line(const boost::system::error_code& err)
  {

    if (!err)
    {

      httpAsync::handle_read_check_response(err);
      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket, response, "\r\n\r\n",
          boost::bind(&httpAsync::handle_read_headers, this,
            boost::asio::placeholders::error));

      
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

 void httpAsync::handle_read_headers(const boost::system::error_code& err)
  {

    if (!err)
    {
      Connection< tcpContext >::handle_read_headers_process();

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket, response,
          boost::asio::transfer_at_least(1),
          boost::bind(&httpAsync::handle_read_content, this,
            boost::asio::placeholders::error));

      
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

void httpAsync::handle_read_content(const boost::system::error_code& err)
  {

    if (!err)
    {
//       response_string_stream.str(std::string(""));
      // Write all of the data that has been read so far.
	response_string_stream<< &response;
      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket, response,
          boost::asio::transfer_at_least(1),
          boost::bind(&httpAsync::handle_read_content, this,
            boost::asio::placeholders::error));
      
    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error: " << err << "\n";
    }
    
            
	response_string_stream<< &response;
      	this->notifyWaitingClient();
  }   
  
  void httpAsync::abort()
{

}

