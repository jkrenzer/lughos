#ifndef TCP_ASYNC_HPP
#define TCP_ASYNC_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

#include <string>
#include <vector>
#include <cstring>

#include "connectionImpl.hpp"
#include "tcpConnections.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "Dict.hpp"


using boost::asio::ip::tcp;

class tcpAsync : virtual public connection<tcpContext>
{
  private:
	tcpAsync(const tcpAsync &p);
	tcpAsync &operator=(const tcpAsync &p); 

// 	tcp::socket socket;


  protected:
    
//     	boost::asio::io_service io_service_;
	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_write_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
// 	void handle_read_check_response(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);
// 	void handle_read_headers_process();
	void handle_read_content(const boost::system::error_code& err);
// 	Dict dict_async;
// 	void compose_request_stream(const std::string &buf);
	
// 	std::string host_path;
// 	std::string server;
	
// 	tcp::socket socket_async;
// 	tcp::resolver resolver;
// 	tcp::resolver::query* query;


	
  public:
	tcpAsync(void);
	~tcpAsync(void);
	
	int write_async(const std::string &buf);

  
};



#endif