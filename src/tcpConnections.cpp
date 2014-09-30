// #include "StdAfx.h"

// #include <Setupapi.h>
// #pragma comment(lib, "Setupapi.lib")

#include "tcpConnections.hpp"
#include "Dict.hpp"
// #include "httpDict.hpp"

connection<tcpContext>::connection(void) :io_service_(),io_service_async(), socket_async(io_service_async), socket(io_service_), resolver(io_service_), resolver_async(io_service_async)
{
  this->query=NULL;
  this->query_async=NULL;
//   dict = new httpDict;
}

connection<tcpContext>::~connection(void)
{
  if (this->query) delete this->query;
  if (this->query_async) delete this->query_async;
	stop();
}


bool connection<tcpContext>::start(const char *server_name)
{
      
    this->server=server_name;
    this->query= new tcp::resolver::query(server_name, "http");
    this->query_async= new tcp::resolver::query(server_name, "http");


    return true;

}



void connection<tcpContext>::reset()
{
}


void connection<tcpContext>::stop()
{

}



void connection<tcpContext>::compose_request_stream(const std::string &buf, Dict * dict)
{
//      std::cout << "request: " << &request_<< "\n";
//   if(dict==NULL)std::cout<<"Nullpointer!"<<std::endl;
    dict->compose_request(this->server, buf, &request_);

//        std::cout << "request: " << &request_<< "\n";
}

void connection<tcpContext>::set_port()
{

}


  
   void connection<tcpContext>::handle_read_check_response(const boost::system::error_code& err, Dict * dict)
  {

    dict->check_response(&response_);
   
  }


void connection<tcpContext>::handle_read_headers_process()
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

std::string connection<tcpContext>::read()
{
        std::string s = response_string_stream.str();

	response_string_stream.str("");
    return s;  

}