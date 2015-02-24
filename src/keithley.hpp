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

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "httpDict.hpp"
#include "measuredValue.hpp"

using namespace lughos;
using boost::asio::ip::tcp;

class KeithleyConnection : public tcpAsync
{
public:
    KeithleyConnection(boost::shared_ptr< boost::asio::io_service > io_service);
};

class Keithley : public Device
{


  
  private:
	Keithley(const Keithley &p);
	Keithley &operator=(const Keithley &p);
	
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	bool isConnectedImplementation();
	measuredValue storedMeasure;
	std::string serverName;
	
  public:
	Keithley(void);
	~Keithley(void);
	measuredValue getMeasure(bool force=false);	

protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
};



#endif