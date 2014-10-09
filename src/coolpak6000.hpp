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
#include "serialSync.hpp"
#include "serialAsync.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "unitValue.hpp"


class coolpak6000 :virtual public serialSync, virtual public serialAsync
{
  private:
	coolpak6000(const coolpak6000 &p);
	coolpak6000 &operator=(const coolpak6000 &p);
	

  public:
	coolpak6000(void);
	~coolpak6000(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);
	virtual void set_default();
	virtual std::string read();
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
	unitValue get_delay();
	unitValue get_operating_hours();
	
protected:
  	void compose_request(const std::string &buf);
	std::string software_version;
	unitValue operating_hours;
	unitValue delay;
	int compressor_state;
	int command_state;
	int coolhead1_state;
	int coolhead2_state;
	int number_of_errors;
	std::string errorstring;
	int number_of_saved_errors;

};



#endif