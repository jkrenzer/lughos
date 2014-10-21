// #include "StdAfx.h"

// #include <Setupapi.h>
// #pragma comment(lib, "Setupapi.lib")

#include "tcpConnections.hpp"
#include "Dict.hpp"
// #include "httpDict.hpp"

Connection<tcpContext>::Connection(void) :io_service_(),io_service_async(), socket_async(io_service_async), socket(io_service_), resolver(io_service_), resolver_async(io_service_async)
{
  this->query=NULL;
  this->query_async=NULL;
//   dict = new httpDict;
}

Connection<tcpContext>::~Connection(void)
{
  if (this->query) delete this->query;
  if (this->query_async) delete this->query_async;
	stop();
}


bool Connection<tcpContext>::start()
{
      
    	if (server_name.empty()) {
		std::cout << "please set server name before start" << std::endl;
		return false;
	}
    this->server=server_name;
    this->query= new tcp::resolver::query(server_name, "http");
    this->query_async= new tcp::resolver::query(server_name, "http");


    return true;

}



void Connection<tcpContext>::reset()
{
}


void Connection<tcpContext>::stop()
{

}



void Connection<tcpContext>::compose_requeststream(const std::string &buf, Dict * dict)
{
//      std::cout << "request: " << &request<< "\n";
//   if(dict==NULL)std::cout<<"Nullpointer!"<<std::endl;
    dict->compose_request(this->server, buf, &request);

//        std::cout << "request: " << &request<< "\n";
}

void Connection<tcpContext>::set_port()
{

}


  
   void Connection<tcpContext>::handle_read_check_response(const boost::system::error_code& err, Dict * dict)
  {

    dict->check_response(&response_);
   
  }


void Connection<tcpContext>::handle_read_headers_process()
{
        // Process the response headers.
      std::istream response_stream(&response_);
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

	response_string_stream.str("");
    return s;  

}