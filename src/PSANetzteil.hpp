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
#include "serialSync.hpp"
#include "serialAsync.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"

using namespace lughos;
class PSANetzteil : public Device
{
  private:
	PSANetzteil(const PSANetzteil &p);
	PSANetzteil &operator=(const PSANetzteil &p);
	
	
  public:
	PSANetzteil();
	~PSANetzteil(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	void off();
	void on();
	std::string get_current();
	std::string get_voltage();
	std::string get_temperature();
// 	bool is_on;
protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);};



#endif