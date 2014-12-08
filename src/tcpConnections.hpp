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

#include <boost/lexical_cast.hpp>

#include <boost/thread/recursive_mutex.hpp>



using boost::asio::ip::tcp;

class tcpContext
{
};
template <> class Connection<tcpContext>: public ConnectionTemplate<tcpContext>
{
  private:
	Connection(const Connection &p);
	Connection &operator=(const Connection &p); 
	void wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error);

  protected:
 	boost::recursive_mutex mutex;
	boost::shared_ptr<boost::asio::io_service>  io_service_;
	boost::shared_ptr<boost::asio::io_service>  io_service_async;
	boost::shared_ptr<tcp::resolver> resolver;
	boost::shared_ptr<tcp::resolver::query> query;
	boost::shared_ptr<tcp::socket> socket;
	boost::shared_ptr<Dict> dict;

	void handle_read_check_response(const boost::system::error_code& err);
	void handle_read_headers_process();

	virtual void compose_request(const std::string &buf);
	boost::asio::deadline_timer timeoutTimer;
	
	std::ostringstream response_string_stream;
	std::string server;
	
	std::string read();
	bool start();
	void stop();
	virtual bool connect() = 0;
	virtual bool disconnect() = 0;
	bool connected;

	
  public:
	Connection(boost::shared_ptr<boost::asio::io_service> io_service) ;
	~Connection(void);

	std::string server_name;
	bool IPv6;
	void set_port(std::string port);
	void reset();

	
	boost::asio::streambuf response;
	boost::asio::streambuf request;

// 	virtual std::string read();
	virtual int write(std::string query);
	virtual int write_only(std::string query);
	std::string response_string;
	virtual bool testconnection();


	std::string port_name;
	virtual void set_default();
		int exp_lenght=1;
		

  
};



#endif