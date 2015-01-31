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
#include "interpolation.hpp"

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
	void switch_on();
	void switch_off();
	float set_voltage_max(float f);
	float set_voltage_min(float f);
	float set_current_lim(float f);
	int set_power_lim(float f);
	measuredValue get_channel(int i, bool force=false);

	float getLimitMaxVoltage();
	float getLimitMaxCurrent();
	float getLimitMinVoltage();
	float getPower();

	bool readout();
	
protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
	bool mode;
	int controler;
	measuredValue channel_output[8];
	measuredValue maxVoltage;
	measuredValue minVoltage;
	measuredValue maxCurrent;
	measuredValue maxPower;
	SplineTransformation unitsToVoltage;
 	SplineTransformation unitsToCurrent;
 	SplineTransformation unitsToPower;
	bool readoutSetting(measuredValue& value, std::string unit, std::string controlChar, std::string answerChar, SplineTransformation& transformation);
	bool readoutChannels();
	std::string floatToBinaryStr(float f, SplineTransformation& transformation);

	
  
};



#endif