#ifndef PSANETZTEIL_HPP
#define PSANETZTEIL_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

#include <string>
#include <vector>
#include <cstring>
#include "serialConnections.hpp"
#include "serialAsync.hpp"
#include "measuredValue.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"

using namespace lughos;

class PSAPowersupplyConnection : public serialAsync
{
public:
  PSAPowersupplyConnection(boost::shared_ptr< boost::asio::io_service > io_service);
  PSAPowersupplyConnection();
};

class PSAPowersupply : public Device
{
  private:
	PSAPowersupply(const PSAPowersupply &p);
	PSAPowersupply &operator=(const PSAPowersupply &p);
	
	
  public:
	PSAPowersupply();
	virtual ~PSAPowersupply(void);
	
	void initImplementation();
	bool isConnectedImplementation();
	void shutdownImplementation();
	void off();
	void on();
	measuredValue get_current();
	measuredValue get_voltage();
	measuredValue get_temperature();
// 	bool is_on;
protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);};



#endif