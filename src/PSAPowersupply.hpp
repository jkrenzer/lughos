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