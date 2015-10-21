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

#ifndef KITHLEIGH_HPP
#define KITHLEIGH_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

#include <string>
#include <vector>
#include <cstring>

#include "connectionImpl.hpp"
#include "tcpConnections.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "httpDict.hpp"
#include "measuredValue.hpp"

using namespace lughos;
using boost::asio::ip::tcp;

class KeithleyConnection : public tcpConnection
{
public:
    KeithleyConnection();
    KeithleyConnection (boost::shared_ptr<boost::asio::io_service> io_service);
};

class Keithley : public Device
{


  
  private:
	Keithley(const Keithley &p);
	Keithley &operator=(const Keithley &p);
	
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	bool isConnectedImplementation();
	measuredValue<double> storedMeasure;
	std::string serverName;
	
  public:
	Keithley(void);
	~Keithley(void);
	measuredValue<double> getMeasure(bool force=false);	

protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
};



#endif