#ifndef MAXIGAUGE_HPP
#define MAXIGAUGE_HPP

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

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "measuredValue.hpp"

using namespace lughos;

class MaxiGauge : public Device
{
  private:
	MaxiGauge(const MaxiGauge &p);
	MaxiGauge &operator=(const MaxiGauge &p);
	measuredValue storedPressure;
	
	
  public:
	MaxiGauge();
	~MaxiGauge(void);
	
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	
	bool sensor_on(int sensor);
	bool sensor_off(int sensor);
	measuredValue getPressure(int sensor, bool force = false);
	std::string get_status(int sensor);
	
protected:
	int sensor_bench[6];
	
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
};



#endif