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
#include "measuredValue.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"

using namespace lughos;

class PSAPowersupplyConnection : public serialConnection
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
	
	exposedMeasurement<double> current;
	exposedMeasurement<double> voltage;
	exposedMeasurement<double> temperature;
	exposedMeasurement<bool> state;
	
protected:	
	void initImplementation();
	bool isConnectedImplementation();
	void shutdownImplementation();
	measuredValue<bool> get_state();
	void set_state(bool state);
	measuredValue<double> get_current();
	measuredValue<double> get_voltage();
	measuredValue<double> get_temperature();
	void memberDeclaration();
// 	bool is_on;

	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);};



#endif