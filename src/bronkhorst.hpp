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
 
  public:
        enum ControlMode {BUSRS232 = '\x00', AnalogInput = '\x01', FLOWBusSlave = '\x02', ValveClose = '\x03', ControllerIdle = '\x04', TestingMode = '\x05', TuningMode = '\x06', Setpoint100percent = '\x07', ValveFullyOpen = '\x08', CalibrationMode = '\x09', AnalogSlave = '\x0a', Setpoint0percent = '\x0b', FLOWBusAnalogSlave = '\x0c', RS232 = '\x12', ValveStearing = '\x14', AnalogValveStearing = '\x15', ValveSaveState = '\x16'};
  private:
	bronkhorst(const bronkhorst &p);
	bronkhorst &operator=(const bronkhorst &p);
	
	mutable Mutex mutex;

	
protected:
	void initImplementation();
	void shutdownImplementation();
	bool isConnectedImplementation();
	measuredValue<double> getSetpoint();
	measuredValue<double> getFlow();
	measuredValue<double> getMaxCapacity();
	measuredValue<ControlMode> getControlMode();
	
	void setSetpoint(measuredValue<double> value);
	void setControlMode(measuredValue<ControlMode> mode);

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
	exposedMeasurement<ControlMode> controlMode;
};



#endif