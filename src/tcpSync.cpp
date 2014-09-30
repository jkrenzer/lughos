#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "tcpConnections.hpp"
#include "Dict.hpp"
#include "tcpSync.hpp"

// tcpSync::tcpSync(void)
// {
// 
// }
tcpSync::tcpSync(void)
{
//   response_string_stream="";
  this->query=NULL; 
//   this->dict=NULL;
}

tcpSync::~tcpSync(void)
{
  if (this->query) delete this->query;
//   if (this->dict) delete this->dict;
	tcpSync::stop();
}



int tcpSync::write(const std::string &buf)
{
 
    tcpSync::compose_request_stream(buf, dict);
     
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
  
    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
  
     
 
  


    boost::asio::write(socket, request_);

    // Read the response status line.
    boost::asio::read_until(socket, response_, "\r\n");

      tcpSync::handle_read_check_response(error, dict);
    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response_, "\r\n\r\n");

    tcpSync::handle_read_headers_process();

    // Read until EOF, writing data to output as we go.
    while (boost::asio::read(socket, response_,
          boost::asio::transfer_at_least(1), error))response_string_stream<< &response_;

    
    if (error != boost::asio::error::eof)
      throw boost::system::system_error(error);
  
        std::string s = response_string_stream.str();
// 	std::cout << s << '\n';
	response_string_stream.str("");
	
	 static const boost::regex e("<body>(.*)</body>");
	 boost::cmatch res;
	 boost::regex_search(s.c_str(), res, e);
	 response_string_stream << res[1] << '\n';
    
    return 1;
}



