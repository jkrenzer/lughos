#ifndef BASIC_CONNECTIONS_HPP
#define BASIC_CONNECTIONS_HPP

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


typedef boost::shared_ptr<boost::asio::serial_port> serial_port_ptr;
using boost::asio::ip::tcp;
using boost::asio::ip::udp;
#define SERIAL_PORT_READ_BUF_SIZE 256


class tcpContext
{
};

template <> class connection<tcpContext>: public connectionTemplate<tcpContext>
{
 
  protected:
	boost::asio::io_service io_service_;

	boost::mutex mutex_;
	char end_of_line_char_;
	char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
	std::string read_buf_str_;



	
  private:
	connection(const connection &p);
	connection &operator=(const connection &p); 
	boost::array<char, 1024> buffer_out;
	tcp::socket socket;
	tcp::socket socket_async;
	tcp::resolver resolver;
	tcp::resolver::query* query;

	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_write_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_check_response(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_headers_process();
	void handle_read_content(const boost::system::error_code& err);

	

	
	void compose_request_stream(const std::string &buf);
	
	std::string host_path;
	std::string server;

	
  public:
	connection(void);
	virtual ~connection(void);

	virtual bool start(const char *server_name);
	virtual void stop();
	virtual void set_port();

	int write(const std::string &buf);
	int write_async(const std::string &buf);
	boost::asio::streambuf response_;
	boost::asio::streambuf request_;
	void reset();
  
};

class httpDict
{
  public:                              // öffentlich
    httpDict();                      // der Default-Konstruktor
    ~httpDict();                     // der Destruktor
 
    void check_response(boost::asio::streambuf* response);  
    void compose_request(std::string server, const std::string &buf, boost::asio::streambuf* request);       

};


#endif