#ifndef BASIC_CONNECTIONS_HPP
#define BASIC_CONNECTIONS_HPP

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

#include <string>
#include <vector>
#include "connectionImpl.hpp"

typedef boost::shared_ptr<boost::asio::serial_port> serial_port_ptr;

#define SERIAL_PORT_READ_BUF_SIZE 256


class serialContext
{
public:
  boost::logic::tribool DCD;
  boost::logic::tribool DTR;
  boost::logic::tribool DSR;
  boost::logic::tribool RTS;
  boost::logic::tribool CTS;
  boost::logic::tribool RI;
  
  serialContext() : DCD(boost::logic::indeterminate),  DTR(boost::logic::indeterminate), DSR(boost::logic::indeterminate),  RTS(boost::logic::indeterminate), CTS(boost::logic::indeterminate), RI(boost::logic::indeterminate)
  {
    
  }
};

template <> class connection<serialContext>: public connectionTemplate<serialContext>
{
 
  protected:
	boost::asio::io_service io_service_;
	serial_port_ptr port_;
	boost::mutex mutex_;
	char end_of_line_char_;
	char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
	std::string read_buf_str_;


	
  private:
	connection(const connection &p);
	connection &operator=(const connection &p); 
	boost::array<char, 1024> buffer_out;
	void wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error);

	
  public:
	connection(void);
	virtual ~connection(void);

	char end_of_line_char() const;
	void end_of_line_char(const char &c);
	std::string readBuffer;

	virtual bool start(const char *port_name, int baud_rate=9600);
	virtual void stop();
	virtual void set_port();

	int write(const std::string &buf);
	void reset();


	
  protected:

	virtual void async_read_some_();
	virtual void on_receive_(const boost::system::error_code& ec, size_t bytes_transferred);
	virtual void on_receive_(const std::string &data);
	int write_some(const char *buf, const int &size);

  
};


#endif