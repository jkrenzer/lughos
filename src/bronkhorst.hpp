#ifndef BRONKHORST_HPP
#define BRONKHORST_HPP

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
#include "bronkhorstMessage.hpp"
#include "device.hpp"
#include "measuredValue.hpp"
#include "utils.hpp"

using namespace lughos;

class bronkhorst : public Device
{
  private:
	bronkhorst(const bronkhorst &p);
	bronkhorst &operator=(const bronkhorst &p);
	
	
public:
	bronkhorst();
	~bronkhorst(void);
	
	template <class T> void setDefaultImpl(T& connection);
	virtual void set_default();
	void initImplementation();
	void shutdownImplementation();
	bool isConnectedImplementation();
	measuredValue get_value();
	std::string set_flow(float value);
protected:
	std::string interpretAnswer(std::string query);
	std::string composeRequest(std::string query);
	float maxCapacity;
	
	measuredValue storedMeasure;
};



#endif