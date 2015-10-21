/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

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
	
	mutable Mutex mutex;
	
  public:
	enum class Mode {Powersupply, BeamCurrentController};
    
	RFG(void);
	virtual ~RFG(void);
	  
	 enum class Controller {Voltage,  Current,  Power};
	 
	 
	 exposedMeasurement<double> target;
	 exposedMeasurement<double> voltage;
	 exposedMeasurement<double> power;
	 exposedMeasurement<double> current;
	 exposedMeasurement<double> temperature;
	 exposedMeasurement<double> currentLimitMax;
	 exposedMeasurement<double> voltageLimitMax;
	 exposedMeasurement<double> voltageLimitMin;
	 exposedMeasurement<bool> output;
         exposedMeasurement<long int> bccFeedbackSignal;
         exposedMeasurement<long int> bccOutputSignal;
         exposedMeasurement<long int> aux1;
         exposedMeasurement<long int> aux2;
	 exposedMeasurement<Mode> mode;
	 exposedMeasurement<Controller> controller;
	 exposedMeasurement<double> resistanceCorrection;
	 
	 
	void set_voltage_max_raw(int i);
	void set_voltage_min_raw(int i);
	void set_current_lim_raw(int i);
	void set_target_value_raw(int i);
	int get_channel_raw(int i, bool force=false);
	 
  protected:
	void initImplementation();
	bool isConnectedImplementation();
	void shutdownImplementation();
	void set_mode(Mode mode);
	void set_controller(Controller controller);
	void set_output(bool mode);
	void set_voltage_max(double f);
	void set_voltage_min(double f);
	void set_current_lim(double f);
	void set_target_value(double f);

	measuredValue<double> get_channel(int i, bool force=false);
	
	measuredValue<double> getLimitMaxVoltage();
	measuredValue<double> getLimitMaxCurrent();
	measuredValue<double> getLimitMinVoltage();
	measuredValue<double> getTargetValue();
	bool readout(bool raw = false);
	double getInteralResistance();
	void setInternalResistance(double resistance);
	
	void memberDeclaration();
	

	std::string interpretAnswer(std::string query) const;
	std::string composeRequest(std::string query) const ;
	measuredValue<double> channel_output[8];
	int channel_output_raw[8];
	SplineTransformation unitsToVoltageReg;
 	SplineTransformation unitsToCurrentReg;
 	SplineTransformation unitsToPowerReg;
	SplineTransformation unitsToVoltageLimMax;
	SplineTransformation unitsToVoltageLimMin;
 	SplineTransformation unitsToCurrentLim;
 	SplineTransformation unitsToPowerMeas;
	SplineTransformation unitsToVoltageMeas;
	SplineTransformation unitsToCurrentMeas;
	unsigned int readoutSetting(std::string controlChar, std::string answerChar);
	bool readoutChannels();
	std::string floatToBinaryStr(float f, SplineTransformation& transformation) const;
	std::string intToBinaryStr(uint16_t i) const;

	
  
};



#endif