
#include "tcpConnections.hpp"

httpDict::httpDict(void) 
{

}

httpDict::~httpDict(void)
{

}

void httpDict::check_response(boost::asio::streambuf* response)
{
      // Check that response is OK.
      std::istream response_stream(response);
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

void httpDict::compose_request(std::string server, const std::string &buf, boost::asio::streambuf* request)
{
    std::ostream request_stream(request);
    
    std::string port_name = server;
     std::string host_path = "/LICENSE_1_0.txt";
//      std::string host_path = "/scpi_response.html?cmd=";
    request_stream << "GET " << host_path.c_str()<<buf.c_str()<< " HTTP/1.0\r\n";
    request_stream << "Host: " <<server<< "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";
    return;
  
}