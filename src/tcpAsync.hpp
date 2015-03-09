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
#include <boost/regex.hpp>

#include <iostream>
#include <boost/array.hpp>
#include "Dict.hpp"



using boost::asio::ip::tcp;

class tcpAsync : virtual public Connection<tcpContext>
{
  private:
	tcpAsync(const tcpAsync &p);
	tcpAsync &operator=(const tcpAsync &p); 

// 	tcp::socket socket;


  protected:
    
//     	boost::asio::io_service io_service_;
	void handle_resolve(boost::function<void()> callback, const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_connect(boost::function<void()> callback, const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator = tcp::resolver::iterator());
	void handle_write_request(boost::regex& regExpr, const boost::system::error_code& err);
// 	void handle_read_status_line(const boost::system::error_code& err);
// 	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_rest(const boost::system::error_code& err);
	void handle_read_content(boost::regex& regExpr,const boost::system::error_code& err);
	void connect(boost::function<void (void)> callback = boost::function<void (void)>());
	void disconnect();
	
	boost::regex endOfLineRegExpr_;
    
  public:
	tcpAsync(boost::shared_ptr<boost::asio::io_service> io_service);
        tcpAsync();
	~tcpAsync(void);
	
	
	
	int write(std::string query, boost::regex regExpr = boost::regex());
	
	void abort();
  
};



#endif