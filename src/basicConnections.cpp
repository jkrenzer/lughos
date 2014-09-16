// #include "StdAfx.h"

// #include <Setupapi.h>
// #pragma comment(lib, "Setupapi.lib")

#include "basicConnections.hpp"


connection<serialContext>::connection(void) : end_of_line_char_('\n')
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

// std::vector<std::string> SerialPort::get_port_names()
// {
// 	std::vector<std::string> names;
// 
// 	BOOL rv;
// 	DWORD size;
// 	GUID guid[1];
// 	HDEVINFO hdevinfo;
// 	DWORD idx = 0;
// 	SP_DEVINFO_DATA devinfo_data;
// 	devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
// 	int count = 0;
// 
// 	rv = SetupDiClassGuidsFromName("Ports", (LPGUID)&guid, 1, &size) ;
// 	if (!rv) {
// 		std::cout << "error : SetupDiClassGuidsFromName() failed..." << std::endl;
// 		return names;
// 	}
// 
// 	hdevinfo = SetupDiGetClassDevs(&guid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
// 	if (hdevinfo == INVALID_HANDLE_VALUE) {
// 		std::cout << "error : SetupDiGetClassDevs() failed..." << std::endl;
// 		return names;
// 	}
// 
// 	while(SetupDiEnumDeviceInfo(hdevinfo, idx++, &devinfo_data)) {
// 		char friendly_name[MAX_PATH];
// 		char port_name[MAX_PATH];
// 		DWORD prop_type;
// 		DWORD type = REG_SZ;
// 		HKEY hKey = NULL;
// 
// 		rv = ::SetupDiGetDeviceRegistryProperty(hdevinfo, &devinfo_data, SPDRP_FRIENDLYNAME, &prop_type,
// 			                                    (LPBYTE)friendly_name, sizeof(friendly_name), &size);
// 		if (!rv) {
// 			std::cout << "error : SetupDiGetDeviceRegistryProperty() failed..." << std::endl;
// 			continue;
// 		}
// 
// 		hKey = ::SetupDiOpenDevRegKey(hdevinfo, &devinfo_data, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
// 		if (!hKey) continue;
// 	
// 		size = sizeof(port_name);
// 		rv = ::RegQueryValueEx(hKey, "PortName", 0, &type, (LPBYTE)&port_name, &size);
// 		::RegCloseKey(hKey);
// 	
// 		names.push_back(port_name);
// 	}
// 
// 	SetupDiDestroyDeviceInfoList(hdevinfo);
// 
// 	return names;
// }

// int SerialPort::get_port_number()
// {
// 	std::vector<std::string> names = get_port_names();
// 	return names.size();
// }
// 
// std::string SerialPort::get_port_name(const unsigned int &idx)
// {
// 	std::vector<std::string> names = get_port_names();
// 	if (idx >= names.size()) return std::string();
// 	return names[idx];
// }

// void SerialPort::print_devices()
// {
// 	std::cout << "SerialPort::print_devices()" << std::endl;
// 	int n = SerialPort::get_port_number();
// 	for (int i = 0; i < n; ++i) {
// 		std::string name = SerialPort::get_port_name(i);
// 		std::cout << "\t" << name.c_str() << std::endl;
// 	}
// }


bool connection<serialContext>::start(const char *com_port_name, int baud_rate)
{
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
	
	this->reset();
	// option settings...
	port_->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
	port_->set_option(boost::asio::serial_port_base::character_size(7));
	port_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
	port_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
	port_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service_));

	async_read_some_();
	
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

int connection<serialContext>::write_some(const char *buf, const int &size)
{
	boost::system::error_code ec;

	if (!port_) return -1;
	if (size == 0) return 0;

	return port_->write_some(boost::asio::buffer(buf, size), ec);
}



void connection<serialContext>::set_port()
{

}
    



void connection<serialContext>::async_read_some_()
{
	if (port_.get() == NULL || !port_->is_open()) return;

	port_->async_read_some( 
		boost::asio::buffer(read_buf_raw_, SERIAL_PORT_READ_BUF_SIZE),
		boost::bind(
			&connection<serialContext>::on_receive_,
			this, boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred));
}


//   std::string SerialPort::sync_read_some()
//   {
//     this->readBuffer.clear();
// //     clear_buffer();
//     this->io_service_.reset();
//     boost::asio::deadline_timer timeout(io_service_);
//     //std::cout << " Prepare reading.";
//     this->io_service_.post(boost::bind(&SerialPort::doRead,this,boost::ref(timeout)));
//     //std::cout << " Ready to run.";
//     this->io_service_.run();  // will block until async callbacks are finished
//     //std::cout << " Buffer: " << this->readBuffer << " Size: " <<  this->readBuffer.size() << " Bytes Transfered: " << boost::asio::placeholders::bytes_transferred << std::endl;
//     return this->readBuffer;
//   }


//     void clear_buffer_()
//     {
//       std::fill(buffer_out.begin() , buffer_out.end(), 0);
//     }
//     

// void SerialPort::doRead(boost::asio::deadline_timer &timeout)
//     {
//       if(port_)
//       {
// 	
// 	//std::cout << " Timout set.";
// 	timeout.expires_from_now(boost::posix_time::milliseconds(this->timeOut));
// 	timeout.async_wait(boost::bind(&SerialPort::wait_callback,this, boost::ref(port_), boost::asio::placeholders::error));
// 	//std::cout << " Reading.";
// // 	this->port_.async_read_some(boost::asio::buffer(this->buffer_out), boost::bind(&SerialPort::read_callback,this,boost::ref(timeout), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
// // 	std::cout << " Received: \"" << buffer_out.c_array() << "\"" << std::endl;
// // 	this->readBuffer.append(buffer_out.c_array());
//       }
//     }

void connection<serialContext>::on_receive_(const boost::system::error_code& ec, size_t bytes_transferred)
{
	
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
    