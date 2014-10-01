// #include "StdAfx.h"

// #include <Setupapi.h>
// #pragma comment(lib, "Setupapi.lib")

#include "basicConnections.hpp"


connection<serialContext>::connection(void) : end_of_line_char_('\r') 
{

}

connection<serialContext>::~connection(void)
{
	stop();
}

char connection<serialContext>::end_of_line_char() const
{
    return this->end_of_line_char_;
}

void connection<serialContext>::end_of_line_char(const char &c)
{
  this->end_of_line_char_ = c;
}

bool connection<serialContext>::start(const char *com_port_name)
{
  return this->start(com_port_name, 9600);
}

bool connection<serialContext>::start(const char *com_port_name, int baud_rate)
{
	end_of_line_char('\r');
	boost::system::error_code ec;

	if (port_) {
		std::cout << "error : port is already opened..." << std::endl;
		return false;
	}

	port_ = serial_port_ptr(new boost::asio::serial_port(io_service_));
	port_->open(com_port_name, ec);
	if (ec) {
		std::cout << "error : port_->open() failed...com_port_name="
			<< com_port_name << ", e=" << ec.message().c_str() << std::endl; 
		return false;
	}
	
// 	this->reset();
	// option settings...
	port_->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	port_->set_option(boost::asio::serial_port_base::character_size(8));
	port_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	port_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
	port_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

	


	
	return true;

}

void connection<serialContext>::reset()
    {
      
      int pid = port_->native();
      // play with RTS & DTR
      int iFlags=0;
// 	std::cout << "iFlags="<< iFlags<< std::endl; 
//       std::cout << "port_type="<< porttype << std::endl; 
      if (context.DCD)
      {
	iFlags = 1;
	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.DCD)
      {
	iFlags = 0;
	ioctl(pid, TIOCMBIC, &iFlags);
      }
      else{
	
      }
      if (context.DTR)
      {
	iFlags = TIOCM_DTR;
// 	std::cout << "iFlags DTR="<< iFlags << std::endl; 
	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.DTR)
      {
	iFlags= TIOCM_DTR;
	ioctl(pid, TIOCMBIC, &iFlags);
      }
      if (context.DSR)
      {
	iFlags = TIOCM_DSR;
	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.DSR)
      {
	iFlags = TIOCM_DSR;
	ioctl(pid, TIOCMBIC, &iFlags);
      }
      if (context.RTS)
      {
	iFlags = TIOCM_RTS;
	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.RTS)
      {
	iFlags = TIOCM_RTS;
// 	std::cout << "iFlags RTS="<< iFlags << std::endl; 
	ioctl(pid, TIOCMBIC, &iFlags);
      }
      if (context.CTS)
      {
	iFlags = TIOCM_CTS;
	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.CTS)
      {
	iFlags = TIOCM_CTS;
	ioctl(pid, TIOCMBIC, &iFlags);
      }
      if (context.RI)
      {
	iFlags = TIOCM_RI;
	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.RI)
      {
	iFlags = TIOCM_RI;
	ioctl(pid, TIOCMBIC, &iFlags);
      }
 
    }


void connection<serialContext>::stop()
{
	boost::mutex::scoped_lock look(mutex_);

	if (port_) {
		port_->cancel();
		port_->close();
		port_.reset();
	}
	io_service_.stop();
	io_service_.reset();
}

int connection<serialContext>::write(const std::string &buf)
{
	return write_some(buf.c_str(), buf.size());
}

int connection<serialContext>::write_async(const std::string &buf)
{
	return 0;
}

int connection<serialContext>::write_some(const char *buf, const int &size)
{
	boost::system::error_code ec;

	if (!port_) return -1;
	if (size == 0) return 0;
	 

   	async_read_some_();
	  io_service_.run();
	return port_->write_some(boost::asio::buffer(buf, size), ec);

}



void connection<serialContext>::set_port()
{

}
    



void connection<serialContext>::async_read_some_()
{
  
//   std::cout<<"bist du versackt?"<<std::endl;
	if (port_.get() == NULL || !port_->is_open()) return;
// std::cout<<"nicht versackt"<<std::endl;
	port_->async_read_some( 
		boost::asio::buffer(read_buf_raw_, SERIAL_PORT_READ_BUF_SIZE),
		boost::bind(
			&connection<serialContext>::on_receive_,
			this, boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred));
	      	  io_service_.run();
}

void connection<serialContext>::on_receive_(const boost::system::error_code& ec, size_t bytes_transferred)
{
  std::cout<<"on_receive_"<<std::endl;
	
	boost::mutex::scoped_lock look(mutex_);

	if (port_.get() == NULL || !port_->is_open()) return;
	if (ec) {
		async_read_some_();
		return;
	}

	for (unsigned int i = 0; i < bytes_transferred; ++i) {
		char c = read_buf_raw_[i];
		if (c == end_of_line_char_) {
			this->on_receive_(read_buf_str_);
			read_buf_str_.clear();
		}
		else {
			read_buf_str_ += c;
		}
	}

	async_read_some_();
}

void connection<serialContext>::on_receive_(const std::string &data)
{
	std::cout << "connection::on_receive_() : " << data << std::endl;
}


  void connection<serialContext>::wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error)
  {
    //std::cout << " Calling wait-handler.";
    if (error)
    {
      // Data was read and this timeout was canceled
      return;
    }
    //std::cout << " Timed out.";
    port_.cancel();  // will cause read_callback to fire with an error
  } 