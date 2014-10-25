#include <iostream>
#include <fstream>

#include "serialConnections.hpp"
// #include "Dict.hpp"
// #include "httpDict.hpp"

#ifdef WIN32
#include <windows.h>
#include <winioctl.h>
#endif

#define GUARD boost::lock_guard<boost::recursive_mutex> guard(mutex);

// connection<serialContext>::connection(void) : end_of_line_char_('\r'), io_service_(), flow_control(), baud_rate(), character_size()
// {
// 
// }

Connection<serialContext>::Connection(boost::shared_ptr<boost::asio::io_service> io_service) : end_of_line_char_('\r'),  flow_control(), baud_rate(), character_size(), timeoutTimer(*io_service)
{
this->io_service_= io_service;

}



Connection<serialContext>::~Connection(void)
{
	stop();
}

char Connection<serialContext>::end_of_line_char() const
{
    return this->end_of_line_char_;
}

void Connection<serialContext>::end_of_line_char(const char &c)
{
  this->end_of_line_char_ = c;
}


bool Connection<serialContext>::start()
{
  std::ofstream ofs ("/home/irina/projects/serialConnection_start.txt", std::ofstream::out);
	ofs << "ready to set following options" << std::endl;
	ofs << "baud_rate: "<<baud_rate.value() << std::endl;
	ofs << "character_size: "<<character_size.value() << std::endl;
	ofs << "stop_bits: "<<stop_bits.value()<< std::endl;
	ofs << "parity: "<<parity.value() << std::endl;
	ofs << "flow_control: "<<flow_control.value() << std::endl;

  std::cout<<"port name: "<< port_name<< std::endl;
  ofs<< character_size.value() << std::endl;
  	if (port_name.empty()) {
		std::cout << "please set port name before start" << std::endl;
		ofs<< "please set port name before start" << std::endl;
		ofs.close();
		stop();
		return false;
	}
  
	this->end_of_line_char(end_of_line_char_);
	boost::system::error_code ec;

	if (port_) {
		std::cout << "error : port is already opened..." << std::endl;
		ofs << "error : port is already opened..." << std::endl;
		ofs.close();
		stop();
		return false;
	}
	port_ = serial_port_ptr(new boost::asio::serial_port(*io_service_));
	port_->open(port_name.c_str(), ec);
	if (ec) {
		std::cout << "error : port_->open() failed...com_port_name="
			<< port_name.c_str() << ", e=" << ec.message().c_str() << std::endl; 
		ofs << "error : port_->open() failed...com_port_name="
			<< port_name.c_str() << ", e=" << ec.message().c_str() << std::endl; 
			ofs.close();
		stop();
		return false;
	}
	
// 	this->reset();
	// option settings...


	  try 
	  {
	      port_->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	  }
	  catch(...)
	  {
	      ofs << "baud rate problems" << std::endl;
	  }

	  try 
	  {
	  port_->set_option(boost::asio::serial_port_base::character_size(character_size.value()));
	  }
	  catch(...)
	  {
	      ofs << "character_size problems" << std::endl;
	  }

	  try 
	  {
	    int i=0;
	  port_->set_option(stop_bits);
	  }
	  catch(...)
	  {
	      ofs << "stop_bits problems" << std::endl;
	  }
	
	
	  try 
	  {
	    int i=0;
	    port_->set_option(boost::asio::serial_port_base::parity(parity));
	  }
	  catch(...)
	  {
	      ofs << "parity problems" << std::endl;
	  }
	
	  try 
	  {
	    port_->set_option(boost::asio::serial_port_base::flow_control(flow_control));	  }
	  catch(...)
	  {
	      ofs << "flow_control problems" << std::endl;
	  }
	



	ofs << "start is fine" << std::endl;
	ofs.flush();
	ofs.close();
// 	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service_));
//   io_service_->run();
	return true;
    
    
}



void Connection<serialContext>::reset()
{
 
#ifdef WIN32

DCB dcb;

HANDLE h_Port = CreateFile(port_name.c_str(),GENERIC_READ |  GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);

GetCommState(h_Port, &dcb);
//       int pid = *port_->native();
      // play with RTS & DTR
      int iFlags=0;
// 	std::cout << "iFlags="<< iFlags<< std::endl; 
//       std::cout << "port_type="<< porttype << std::endl; 
      if (context.DCD)
      {
	iFlags = 1;
// 	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.DCD)
      {
	iFlags = 0;
// 	ioctl(pid, TIOCMBIC, &iFlags);
      }
      else{
	
      }
      if (context.DTR)
      {
// 	std::cout << "iFlags DTR="<< iFlags << std::endl; 
	dcb.fDtrControl =DTR_CONTROL_ENABLE;
// 	ioctl(pid, TIOCMBIS, &iFlags);
      }
      else if (!context.DTR)
      {
	dcb.fDtrControl =DTR_CONTROL_DISABLE;
      }
      if (context.DSR)
      {

      }
      else if (!context.DSR)
      {

      }
      if (context.RTS)
      {
	dcb.fRtsControl = RTS_CONTROL_ENABLE;

      }
      else if (!context.RTS)
      {
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
      }
      if (context.CTS)
      {

      }
      else if (!context.CTS)
      {

      }
      if (context.RI)
      {

      }
      else if (!context.RI)
      {

      }

  
#else


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
 
#endif 
}


void Connection<serialContext>::stop()
{
// 	boost::mutex::scoped_lock look(mutex_)
//   std::cout<<"trying to close port"<<std::endl;
    try
    {
	if (port_) 
	{  
// 		std::cout<<"open port found"<<std::endl;

		port_->cancel();
		port_->close();
		port_.reset();

	}
    }
    catch(...)
    {
    }
// 	io_service_->stop();
// 	io_service_->reset();
}



void Connection<serialContext>::compose_request(const std::string &buf)
{
//   	                std::cout<<"wrong compose"<<std::endl;
}

void Connection<serialContext>::set_port()
{

}


  
void Connection<serialContext>::handle_read_check_response(const boost::system::error_code& err)
  {
  
  }


void Connection<serialContext>::handle_read_headers_process()
{
        // Process the response headers.
//       std::istream response_stream(&response_);
//       std::string header;
//       while (std::getline(response_stream, header) && header != "\r");
// 	std::cout << header << "\n";
//       std::cout << "\n";
      
     // Write whatever content we already have to output.
//       if (response_.size() > 0) //response_string_stream<<&response_;
//         std::cout << &response_;
//       response_string_stream
      

}

std::string Connection<serialContext>::read()
{
//         std::string s = response_string_stream.str();
// 	response_string_stream.str("");
// 	stop();
    return response_string_stream.str();  

}

void Connection<serialContext>::set_baud_rate(const int baud)
{
    
  baud_rate=boost::asio::serial_port_base::baud_rate(baud);

}
void Connection<serialContext>::set_character_size(const int size)
{
    
  character_size=boost::asio::serial_port_base::character_size(size);

}

void Connection<serialContext>::set_flow_controll(flow_constroll_bit controll_type)
{
    
  switch(controll_type)
  {
  case off:  // (can I just type case EASY?)
	  flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
  break;

  case software: 
	  flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::software);
  break;

  case hardware: flow_control=boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::hardware);

  default:;

  }
}

void Connection<serialContext>::set_parity(parity_bit parity_type)
{
    
  switch(parity_type)
  {
  case none:  
	parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
      break;

  case odd: 
	 parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::odd);
      break;

  case even:parity=boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even);

  default:;

  }
}

void Connection<serialContext>::set_stop_bits(stop_bits_num stop_bits_type)
{
    
  switch(stop_bits_type)
  {
  case one:  
	stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
      break;

  case onepointfive: 
	 stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::onepointfive);
      break;

  case two: stop_bits=boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::two);

  default:;

  }
}



void Connection<serialContext>::set_default()
{
    
}



bool Connection<serialContext>::testconnection()
{
 GUARD
  bool ConnectionEstablished = false;
     try 
     {
      ConnectionEstablished = this->start();
     }
     catch(...)
     {
       ConnectionEstablished = false;
     }
     if(ConnectionEstablished)
     {
       this->stop();
     }
     return ConnectionEstablished;
}

int Connection<serialContext>::write(std::string query)
{
  return 0;  
}