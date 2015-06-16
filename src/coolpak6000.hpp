#ifndef COOLPACK6000_HPP
#define COOLPACK6000_HPP

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
#include "unitValue.hpp"
#include "device.hpp"
using namespace lughos;
class coolpak6000 : public Device
{
  private:
	coolpak6000(const coolpak6000 &p);
	coolpak6000 &operator=(const coolpak6000 &p);
	

  public:
	coolpak6000();
	~coolpak6000(void);
	
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	bool isConnectedImplementation();
	
	bool compressor_on();
	bool compressor_off();
	bool coolhead_on(int head);
	bool coolhead_off(int head);
	std::string get_data();
	std::string get_error_list();
	std::string get_software_version();
	int get_compressor_state();
	int get_command_state();
	int get_coolhead1_state();
	int get_coolhead2_state();
	int get_number_of_errors();
	std::string get_errorstring();
	int get_number_of_saved_errors();
	unitValue<double> get_delay();
	unitValue<double> get_operating_hours();

		
protected:
  
	std::string software_version;
	unitValue<double> operating_hours;
	unitValue<double> delay;
	int compressor_state;
	int command_state;
	int coolhead1_state;
	int coolhead2_state;
	int number_of_errors;
	std::string errorstring;
	int number_of_saved_errors;
	
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);

};



#endif