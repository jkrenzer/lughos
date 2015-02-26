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

class RFGConnection : public serialAsync
{
public:
    RFGConnection(boost::shared_ptr< boost::asio::io_service > io_service);
};

class RFG :public Device
{
  private:
	RFG(const RFG &p);
	RFG &operator=(const RFG &p);
	
	
  public:
	enum ControllerMode {Voltage, Current, Power};
    
	RFG(void);
	virtual ~RFG(void);

	void initImplementation();
	bool isConnectedImplementation();
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
	float set_target_value(float f);
	int set_voltage_max_raw(int i);
	int set_voltage_min_raw(int i);
	int set_current_lim_raw(int i);
	int set_target_value_raw(int i);
	measuredValue get_channel(int i, bool force=false);
	
	float getLimitMaxVoltage();
	float getLimitMaxCurrent();
	float getLimitMinVoltage();
	float getTargetValue();
	bool readout(bool raw = false);
	double getInteralResistance();
	void setInternalResistance(double resistance);
	
protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
	bool bccMode;
	double internalResistance;
	ControllerMode controllerMode;
	measuredValue channel_output[8];
	measuredValue maxVoltage;
	measuredValue minVoltage;
	measuredValue maxCurrent;
	measuredValue maxPower;
	SplineTransformation unitsToVoltageReg;
 	SplineTransformation unitsToCurrentReg;
 	SplineTransformation unitsToPowerReg;
	SplineTransformation unitsToVoltageLimMax;
	SplineTransformation unitsToVoltageLimMin;
 	SplineTransformation unitsToCurrentLim;
 	SplineTransformation unitsToPowerLim;
	SplineTransformation unitsToVoltageMeas;
	SplineTransformation unitsToCurrentMeas;
	bool readoutSetting(measuredValue& value, std::string unit, std::string controlChar, std::string answerChar, SplineTransformation& transformation, bool raw = false);
	bool readoutChannels();
	std::string floatToBinaryStr(float f, SplineTransformation& transformation);
	std::string intToBinaryStr(uint16_t i);

	
  
};



#endif