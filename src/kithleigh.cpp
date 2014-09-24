// #include "StdAfx.h"

// #include <Setupapi.h>
// #pragma comment(lib, "Setupapi.lib")

#include "kithleigh.hpp"


kithleigh::kithleigh(void)
{
    this->dict = new httpDict;
    this->dict->addition = std::string("/scpi_response.html?cmd=");

}

kithleigh::~kithleigh(void)
{

}

// void kithleigh::compose_request(std::string server, const std::string &buf, boost::asio::streambuf* request)
// {
//     std::ostream request_stream(request);
//       host_path=addition;
//     std::string port_name = server;
// //      std::string host_path = "/LICENSE_1_0.txt";
// //     std::string 
//     request_stream << "GET " << host_path.c_str()<<buf.c_str()<< " HTTP/1.0\r\n";
//     request_stream << "Host: " <<server<< "\r\n";
//     request_stream << "Accept: */*\r\n";
//     request_stream << "Connection: close\r\n\r\n";
//     
//         std::cout << "GET " << host_path.c_str()<<buf.c_str()<< " HTTP/1.0\r\n";
//      std::cout  << "Host: " <<server<< "\r\n";
//      std::cout  << "Accept: */*\r\n";
//      std::cout  << "Connection: close\r\n\r\n";
//     return;
//   
// }
