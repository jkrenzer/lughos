#include <iostream>
#include <fstream>

#include "tcpConnections.hpp"
#include "Dict.hpp"

#ifdef WIN32
#include <windows.h>
#include <winioctl.h>
#endif

#define GUARD boost::lock_guard<boost::recursive_mutex> guard(mutex);



Connection<tcpContext>::Connection(boost::shared_ptr<boost::asio::io_service> io_service) :timeoutTimer(*io_service), request(), response()
{
this->io_service_= io_service;
this->connected = false;
}



Connection<tcpContext>::~Connection(void)
{
// 	stop();
}


bool Connection<tcpContext>::start()
{
//       server_name = port_name;
    	if (server_name.empty()||port_name.empty()) {
		std::cout << "please set server/port name before start" << std::endl;
		return false;
	}
//     server=server_name;
    std::cout << "server: "<<server_name<<" port: "<<port_name << std::endl;
    resolver= boost::shared_ptr<tcp::resolver>(new tcp::resolver(*this->io_service_));
    query= boost::shared_ptr<tcp::resolver::query>(new tcp::resolver::query(server_name, port_name));
    socket= boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(*this->io_service_));
    this->connected = false;
    return true;
    
  
	
}



void Connection<tcpContext>::reset()
{
}


void Connection<tcpContext>::stop()
{

    try
    {
	resolver->cancel();
      if (query) 
	{  

	}
    }
    catch(...)
    {
      std::cout<<"stop failed"<<std::endl;
    }

}



void Connection<tcpContext>::compose_request(const std::string &buf)
{

}


void Connection<tcpContext>::set_port(std::string port)
{
  port_name=port;
}


  
 void Connection<tcpContext>::handle_read_check_response(const boost::system::error_code& err)
  {

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
        return;
      }
      if (status_code != 200)
      {
        std::cout << "Response returned with status code ";
        std::cout << status_code << "\n";
        return;
      }
   
}


void Connection<tcpContext>::handle_read_headers_process()
{
        // Process the response headers.
      std::istream response_stream(&response);
      std::string header;
      while (std::getline(response_stream, header) && header != "\r");
// 	std::cout << header << "\n";
//       std::cout << "\n";
      
     // Write whatever content we already have to output.
//       if (response_.size() > 0) //response_string_stream<<&response_;
//         std::cout << &response_;
//       response_string_stream
      

}

std::string Connection<tcpContext>::read()
{
        std::string s = response_string_stream.str();
	response_string_stream.str(std::string(""));
// 	stop();
    return s;  

}

bool Connection<tcpContext>::testconnection()
{
 GUARD
  bool ConnectionEstablished = false;
     try 
     {
      ConnectionEstablished = this->start();
     }
     catch(...)
     {
       ConnectionEstablished = false;
     }
     if(ConnectionEstablished)
     {
       this->stop();
     }
     return ConnectionEstablished;
}

int Connection<tcpContext>::write(std::string query)
{
  return 0;  
}

int Connection<tcpContext>::write_only(std::string query)
{
  return 0;  
}


void Connection<tcpContext>::set_default()
{
    
}