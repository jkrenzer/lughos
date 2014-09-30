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
// #include "Dict.hpp"


class MaxiGauge :virtual public serialSync
{
  private:
	MaxiGauge(const MaxiGauge &p);
	MaxiGauge &operator=(const MaxiGauge &p);
	
	
  public:
	MaxiGauge(void);
	~MaxiGauge(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);
protected:
  	void compose_request(const std::string &buf);
	void handle_read_check_response(const boost::system::error_code& err);
// 	void handle_read_check_response();
};



#endif