#ifndef KITHLEIGH_HPP
#define KITHLEIGH_HPP

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
#include "tcpAsync.hpp"
#include "tcpSync.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "httpDict.hpp"


using boost::asio::ip::tcp;

class kithleigh :virtual public tcpSync, virtual public tcpAsync
{
  private:
	kithleigh(const kithleigh &p);
	kithleigh &operator=(const kithleigh &p);
	
  public:
	kithleigh(void);
	~kithleigh(void);
	virtual std::string inputoutput(const std::string input, const int async=0);	

protected:
//   	void compose_request(std::string server, const std::string &buf, boost::asio::streambuf* request);
};



#endif