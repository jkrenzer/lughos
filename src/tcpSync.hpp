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
#include "tcpConnections.hpp"

#include <iostream>
#include <boost/array.hpp>
// #include "Dict.hpp"


using boost::asio::ip::tcp;

class tcpSync : virtual public Connection<tcpContext>
{
  private:
	tcpSync(const tcpSync &p);
	tcpSync &operator=(const tcpSync &p); 
// 	tcp::socket socket;


  protected:
    
//     	boost::asio::io_service io_service_;

	
// 	tcp::socket socket;
// 	tcp::resolver resolver;
// 	tcp::resolver::query* query;


	
  public:
	tcpSync(void);
	~tcpSync(void);
	
	int write(const std::string &buf);
// 	void reset();
  
};



#endif