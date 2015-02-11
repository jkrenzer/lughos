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
#include "serialConnections.hpp"
#include "serialAsync.hpp"

#include <iostream>
#include <boost/array.hpp>
#include "device.hpp"
#include "measuredValue.hpp"

using namespace lughos;



class kithleighSerial : public Device
{
  private:
	kithleighSerial(const kithleighSerial &p);
	kithleighSerial &operator=(const kithleighSerial &p);
	
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	measuredValue storedMeasure;
	
	
  public:
	kithleighSerial();
	~kithleighSerial(void);
	
	measuredValue getMeasure(bool force=false);

	
protected:
//   	void compose_request(const std::string &buf);
// 	void handle_read_check_response();
	
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
};



#endif