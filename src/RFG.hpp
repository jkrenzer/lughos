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

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "measuredValue.hpp"
#include "interpolation.hpp"

using namespace lughos;

class RFGConnection : public serialConnection
{
public:
    RFGConnection(boost::shared_ptr< boost::asio::io_service > io_service);
    RFGConnection();
};

class RFG :public Device
{
  private:
	RFG(const RFG &p);
	RFG &operator=(const RFG &p);
	
	
  public:
	enum class Mode {Powersupply, BeamCurrentController};
    
	RFG(void);
	virtual ~RFG(void);
	  
	 enum class Controller {Voltage,  Current,  Power};
	 
	 
	 exposedMeasurement<double> target;
	 exposedMeasurement<double> voltage;
	 exposedMeasurement<double> power;
	 exposedMeasurement<double> current;
	 exposedMeasurement<double> currentLimitMax;
	 exposedMeasurement<double> voltageLimitMax;
	 exposedMeasurement<double> voltageLimitMin;
	 exposedMeasurement<bool> output;
	 exposedMeasurement<Mode> mode;
	 exposedMeasurement<Controller> controller;
	 exposedMeasurement<double> resistanceCorrection;
	 
	 
	int set_voltage_max_raw(int i);
	int set_voltage_min_raw(int i);
	int set_current_lim_raw(int i);
	int set_target_value_raw(int i);
	int get_channel_raw(int i, bool force=false);
	 
  protected:
	void initImplementation();
	bool isConnectedImplementation();
	void shutdownImplementation();
	void set_mode(Mode mode);
	void set_controller(Controller controller);
	void set_output(bool mode);
	float set_voltage_max(float f);
	float set_voltage_min(float f);
	float set_current_lim(float f);
	float set_target_value(float f);

	measuredValue<double> get_channel(int i, bool force=false);
	
	measuredValue<double> getLimitMaxVoltage();
	measuredValue<double> getLimitMaxCurrent();
	measuredValue<double> getLimitMinVoltage();
	measuredValue<double> getTargetValue();
	bool readout(bool raw = false);
	double getInteralResistance();
	void setInternalResistance(double resistance);
	

	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
// 	bool bccMode;
// 	double internalResistance;
// 	Controller controllerMode;
	measuredValue<double> channel_output[8];
	int channel_output_raw[8];
// 	measuredValue<double> maxVoltage;
// 	measuredValue<double> minVoltage;
// 	measuredValue<double> maxCurrent;
// 	measuredValue<double> maxPower;
	SplineTransformation unitsToVoltageReg;
 	SplineTransformation unitsToCurrentReg;
 	SplineTransformation unitsToPowerReg;
	SplineTransformation unitsToVoltageLimMax;
	SplineTransformation unitsToVoltageLimMin;
 	SplineTransformation unitsToCurrentLim;
 	SplineTransformation unitsToPowerLim;
	SplineTransformation unitsToVoltageMeas;
	SplineTransformation unitsToCurrentMeas;
	bool readoutSetting(measuredValue<double>& value, std::string unit, std::string controlChar, std::string answerChar, SplineTransformation& transformation, bool raw = false);
	bool readoutChannels();
	std::string floatToBinaryStr(float f, SplineTransformation& transformation);
	std::string intToBinaryStr(uint16_t i);

	
  
};



#endif