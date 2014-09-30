#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "tcpConnections.hpp"
#include "Dict.hpp"
#include "tcpAsync.hpp"


tcpAsync::tcpAsync(void) 
{
//   response_string_stream="";
  this->query_async=NULL;
  this->dict=NULL;
}

tcpAsync::~tcpAsync(void)
{
  if (this->query_async) delete this->query;
  if (this->dict) delete this->dict;
	tcpAsync::stop();
}



int tcpAsync::write_async(const std::string &buf)
{    
    tcpAsync::compose_request_stream(buf, dict);


//  tcpAsync::handle_resolve(this, boost::asio::placeholders::error, boost::asio::placeholders::iterator);
 resolver_async.async_resolve(*this->query_async, boost::bind(&tcpAsync::handle_resolve, this,
          boost::asio::placeholders::error, boost::asio::placeholders::iterator));
    io_service_async.run();
    
        
    
        std::string s = response_string_stream.str();
	response_string_stream.str("");
	

	 static const boost::regex e("<body>(.*)</body>");
	 boost::cmatch res;
	 boost::regex_search(s.c_str(), res, e);
	 response_string_stream << res[1] << '\n';
	  
  return 1;
}




//-------
void tcpAsync::handle_resolve(const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
  {

    if (!err)
    {

      tcp::endpoint endpoint = *endpoint_iterator;
      socket_async.async_connect(endpoint,
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

    if (!err)
    {
      // The connection was successful. Send the request.
      boost::asio::async_write(socket_async, request_,
          boost::bind(&tcpAsync::handle_write_request, this,
            boost::asio::placeholders::error));
    }
    else if (endpoint_iterator != tcp::resolver::iterator())
    {
      // The connection failed. Try the next endpoint in the list.
      socket_async.close();
      tcp::endpoint endpoint = *endpoint_iterator;
      socket_async.async_connect(endpoint,
          boost::bind(&tcpAsync::handle_connect, this,
            boost::asio::placeholders::error, ++endpoint_iterator));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
    

  }

void tcpAsync::handle_write_request(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Read the response status line.
      boost::asio::async_read_until(socket_async, response_, "\r\n",
          boost::bind(&tcpAsync::handle_read_status_line, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }
  
  

  void tcpAsync::handle_read_status_line(const boost::system::error_code& err)
  {

    if (!err)
    {
      tcpAsync::handle_read_check_response(err, dict);
      // Read the response headers, which are terminated by a blank line.
      boost::asio::async_read_until(socket_async, response_, "\r\n\r\n",
          boost::bind(&tcpAsync::handle_read_headers, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

 void tcpAsync::handle_read_headers(const boost::system::error_code& err)
  {

    if (!err)
    {
      connection< tcpContext >::handle_read_headers_process();

      // Start reading remaining data until EOF.
      boost::asio::async_read(socket_async, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&tcpAsync::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err << "\n";
    }
  }

void tcpAsync::handle_read_content(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Write all of the data that has been read so far.
	response_string_stream<< &response_;
      // Continue reading remaining data until EOF.
      boost::asio::async_read(socket_async, response_,
          boost::asio::transfer_at_least(1),
          boost::bind(&tcpAsync::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error: " << err << "\n";
    }

  }   