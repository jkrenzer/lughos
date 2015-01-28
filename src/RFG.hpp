#ifndef RFG_HPP
#define RFG_HPP

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
#include "serialSync.hpp"
#include "serialAsync.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "measuredValue.hpp"

using namespace lughos;



class RFG :public Device
{
  private:
	RFG(const RFG &p);
	RFG &operator=(const RFG &p);
	
	
  public:
	RFG(void);
	~RFG(void);
	
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	void power_supply_mode();
	void bcc_mode();
	void use_voltage_controler();
	void use_current_controler();
	void use_power_controler();
	float set_voltage_max(std::string f);
	float set_voltage_min(std::string f);
	float set_current_lim(std::string f);
	int set_controler_chanel(int i);
	measuredValue get_channel(int i, bool force=false);

	float getLimitMaxVoltage();
	float getLimitMaxCurrent();
	float getLimitMinVoltage();

	bool readout();
	
protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
	float voltage_max;
	float voltage_min;
	float current_max;
	bool mode;
	int controler;
	measuredValue channel_output[8];
  
};



#endif