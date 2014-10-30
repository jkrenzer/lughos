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
	int set_power_max(int i);
	int set_power_min(int i);
	int set_current_lim(int i);
	int set_controler_chanel(int i);
	measuredValue channel_output[8];
	bool readout();
	
protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
	int power_max;
	int power_min;
	bool mode;
	int controler;
  
};



#endif