#ifndef TCP_CONNECTIONS_HPP
#define TCP_CONNECTIONS_HPP

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

#include <iostream>
#include <boost/array.hpp>
#include "Dict.hpp"


using boost::asio::ip::tcp;

class tcpContext
{
};

template <> class connection<tcpContext>: public connectionTemplate<tcpContext>
{
  private:
	connection(const connection &p);
	connection &operator=(const connection &p); 




  protected:
 
	boost::asio::io_service io_service_;
	boost::asio::io_service io_service_async;
    	tcp::resolver resolver;
	tcp::resolver resolver_async;
	tcp::resolver::query* query;
	tcp::resolver::query* query_async;
    	tcp::socket socket;
	tcp::socket socket_async;
	Dict* dict;

// 	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
// 	void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
// 	void handle_write_request(const boost::system::error_code& err);
// 	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_check_response(const boost::system::error_code& err, Dict* dict);
// 	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_headers_process();
// 	void handle_read_content(const boost::system::error_code& err);
	void compose_request_stream(const std::string &buf, Dict* dict);
	
	boost::asio::streambuf response_;
	boost::asio::streambuf request_;
	std::ostringstream response_string_stream;
	std::string server;


	
  public:
	connection(void);
	~connection(void);

	bool start(const char *server_name);
	void stop();
	void set_port();
	std::string response_string;

	void reset();
  
};



#endif