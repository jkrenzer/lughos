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
// #include "Dict.hpp"


class PSANetzteil :virtual public serialSync
{
  private:
	PSANetzteil(const PSANetzteil &p);
	PSANetzteil &operator=(const PSANetzteil &p);
	
	
  public:
	PSANetzteil(boost::asio::io_service* io_service);
	~PSANetzteil(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);
	virtual void set_default();
	void off();
	void on();
	std::string get_current();
	std::string get_voltage();
	std::string get_temperature();
// 	bool is_on;
protected:
  	void compose_request(const std::string &buf);
// 	void handle_read_check_response();
};



#endif