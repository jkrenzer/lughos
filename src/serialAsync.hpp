#ifndef SERIAL_ASYNC_HPP
#define SERIAL_ASYNC_HPP

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
#include "serialConnections.hpp"

#include <iostream>
#include <boost/array.hpp>
// #include "Dict.hpp"



class serialAsync : virtual public Connection<serialContext>
{
  private:
	serialAsync(const serialAsync &p);
	serialAsync &operator=(const serialAsync &p); 

	void wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error);


  protected:
    
	virtual void handle_write_only(const boost::system::error_code& err);
// 	virtual void on_receive_(const boost::system::error_code& ec, size_t bytes_transferred);
// 	virtual void on_receive_(const std::string &data);
	int write_some(const char *buf, const int &size);
	int write_some_async(const char *buf, const int &size);
	
	
	void handle_write_request(const boost::system::error_code& err);
// 	void handle_read_status_line(const boost::system::error_code& err);
// 	void handle_read_check_response(const boost::system::error_code& err);
// 	void handle_read_headers(const boost::system::error_code& err);
// 	void handle_read_headers_process();
	void handle_read_content(const boost::system::error_code& err);


	
  public:
	serialAsync(boost::shared_ptr<boost::asio::io_service> io_service) ;
	~serialAsync(void);
	
	int write(std::string query);
	int write_only(std::string query);
	void abort();
protected:
/////////////
	std::deque<char> write_msgs_;
	void do_close(const boost::system::error_code& error);
	void write_complete(const boost::system::error_code& error);
	void write_start(void);
	void do_write(const char msg);
	void read_complete(const boost::system::error_code& error, size_t bytes_transferred);
	void read_start(void);
////////////
  
};



#endif