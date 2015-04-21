#ifndef BRONKHORST_HPP
#define BRONKHORST_HPP

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

#include <iostream>
#include <boost/array.hpp>
#include "bronkhorstMessage.hpp"
#include "device.hpp"
#include "measuredValue.hpp"
#include "utils.hpp"

using namespace lughos;

class bronkhorstConnection : public serialConnection
{
public:
    bronkhorstConnection(boost::shared_ptr<boost::asio::io_service> io_service);
    bronkhorstConnection();
};

class bronkhorst : public Device
{
  private:
	bronkhorst(const bronkhorst &p);
	bronkhorst &operator=(const bronkhorst &p);
	

	
protected:
	void initImplementation();
	void shutdownImplementation();
	bool isConnectedImplementation();
	measuredValue<double> get_setpoint();
	measuredValue<double> get_flow();
	measuredValue<double> getMaxCapacity();
	void set_setpoint(measuredValue<double> value);

	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
	measuredValue<double> storedMeasure;
	
	void memberDeclaration();
	
  public:
        bronkhorst();
	virtual ~bronkhorst(void);
	exposedMeasurement<double> setpoint;
	exposedMeasurement<double> flow;
	exposedMeasurement<double> capacity;
};



#endif