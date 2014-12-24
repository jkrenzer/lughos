#ifndef HTTP_ASYNC_HPP
#define HTTP_ASYNC_HPP

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

class httpAsync : virtual public Connection<tcpContext>
{
  private:
	httpAsync(const httpAsync &p);
	httpAsync &operator=(const httpAsync &p); 
	bool writeonly=false;

// 	tcp::socket socket;


  protected:
    
//     	boost::asio::io_service io_service_;
	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_write_request(const boost::system::error_code& err);
	void handle_write_only_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_content(const boost::system::error_code& err);


	

	
  public:
	httpAsync(boost::shared_ptr<boost::asio::io_service> io_service);
	~httpAsync(void);
	
	int write(std::string query);
	int write_only(std::string query);
	
	void abort();
  
};



#endif