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
#include "serialSync.hpp"
#include "serialAsync.hpp"

#include <iostream>
#include <boost/array.hpp>
// #include "Dict.hpp"


class Relais :virtual public serialSync, virtual public serialAsync
{
  private:
	Relais(const Relais &p);
	Relais &operator=(const Relais &p);
	
	
  public:
	Relais(void);
	~Relais(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);
	virtual void set_default();
	virtual std::string read();
	bool input_status(int sensor);
	bool input_on(int sensor);
	bool input_off(int sensor);
	std::string input_read();
	
protected:
  	void compose_request(const std::string &buf);
	int input_bench[8];
};



#endif