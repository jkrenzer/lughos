#ifndef SERIAL_CONNECTIONS_HPP
#define SERIAL_CONNECTIONS_HPP

#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

#include <string>
#include <vector>
#include "connectionImpl.hpp"

typedef boost::shared_ptr<boost::asio::serial_port> serial_port_ptr;

#define SERIAL_PORT_READ_BUF_SIZE 256
enum flow_constroll_bit {off, software, hardware};
enum parity_bit { none, odd, even };
enum stop_bits_num { one, onepointfive, two };
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

	serial_port_ptr port_;
// 	boost::mutex mutex_;
	char end_of_line_char_;
	char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
	std::string read_buf_str_;

// 	char end_of_line;
 
	boost::asio::io_service * io_service_;
	
	boost::asio::serial_port_base::flow_control::type flow_control;
	boost::asio::serial_port_base::character_size character_size;
	boost::asio::serial_port_base::baud_rate baud_rate;
	boost::asio::serial_port_base::parity::type parity;
	boost::asio::serial_port_base::stop_bits::type stop_bits;
	
// 	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
// 	void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
// 	void handle_write_request(const boost::system::error_code& err);
// 	void handle_read_status_line(const boost::system::error_code& err);
	virtual void handle_read_check_response(const boost::system::error_code& err);
// 	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_headers_process();
// 	void handle_read_content(const boost::system::error_code& err);
	virtual void compose_request(const std::string &buf);
	
	
	
	std::stringstream response_string_stream;
// 	const char end_of_line;

private:
  	connection(const connection &p);
	connection &operator=(const connection &p); 
	void wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error);
	char end_of_line_char() const;
	void end_of_line_char(const char &c);



	
  public:
	connection(boost::asio::io_service* io_service);
	~connection(void);
	
	boost::asio::streambuf response_;
	boost::asio::streambuf request_;

	bool start();
	void stop();
	void set_port();
	virtual std::string read();
	std::string response_string;
	
// 	int write(const std::string &buf);
// 	int write_async(const std::string &buf);
	void reset();
	void set_baud_rate(const int baud_rate);
	void set_character_size(const int character_size);
	void set_flow_controll(flow_constroll_bit controll_type);
	void set_parity(parity_bit parity_type);
	void set_stop_bits(stop_bits_num stop);
	std::string port_name;
	virtual void set_default();
		int exp_lenght=1;
  
};



#endif