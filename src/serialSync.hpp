#ifndef TCP_SYNC_HPP
#define TCP_SYNC_HPP

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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <boost/iterator.hpp>
#include <string>
//LATERON ADD SUPPORT FOR CROSSCOMPILING!
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
# define BOOST_ASIO_OPTION_STORAGE DCB
#else
# define BOOST_ASIO_OPTION_STORAGE termios
#include <sys/ioctl.h>
#endif

typedef boost::shared_ptr<boost::asio::serial_port> serial_port_ptr;

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 




#define SERIAL_PORT_READ_BUF_SIZE 256
// #include "Dict.hpp"


using boost::asio::ip::tcp;

class serialSync : virtual public connection<serialContext>
{
  private:
	serialSync(const serialSync &p);
	serialSync &operator=(const serialSync &p); 

// 	tcp::socket socket;


  protected:
//    int baud_rate;
//    boost::asio::serial_port_base::flow_control flow_control;
//    boost::asio::serial_port_base::character_size character_size;
    
//     	boost::asio::io_service io_service_;

	
// 	tcp::socket socket;
// 	tcp::resolver resolver;
// 	tcp::resolver::query* query;


	
  public:
	serialSync(void);
	~serialSync(void);
	
	int write(const std::string &buf);

// 	void reset();
  
};



#endif