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

#ifndef RELAIS_HPP
#define RELAIS_HPP

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
#include <bitset>
#include <boost/array.hpp>
#include "device.hpp"

using namespace lughos;

class RelaisConnection : public serialConnection
{
public:
    RelaisConnection(boost::shared_ptr< boost::asio::io_service > io_service);
    RelaisConnection();
};

class Relais :public Device
{
  private:
	Relais(const Relais &p);
	Relais &operator=(const Relais &p);
	
	
  public:
	Relais();
	virtual ~Relais(void);
	
	exposedMeasurement<std::bitset<8> > relais;
	exposedMeasurement<std::bitset<8> > sensors;
	
protected:
	
	void memberDeclaration();
	enum Module {RelaisModule = 0, SensorModule = 1};

	void initImplementation();
	void shutdownImplementation();
	bool isConnectedImplementation();
	
	virtual measuredValue<std::bitset<8> > get_relais();
	virtual void set_relais(measuredValue<std::bitset<8> > relais_);
	virtual measuredValue<std::bitset<8> > get_sensors();
	std::bitset<8> get_state_of(Module module);
/*	
	bool input_status(int sensor);
	bool input_on(int sensor);
	bool input_off(int sensor);
	std::string input_read();*/
	

	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
// 	std::bitset<8> channel_bench;
// 	std::bitset<8> sensor_bench;
};



#endif