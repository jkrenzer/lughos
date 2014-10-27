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
// #include "Dict.hpp"


class RFG :virtual public serialSync, virtual public serialAsync
{
  private:
	RFG(const RFG &p);
	RFG &operator=(const RFG &p);
	
	
  public:
	RFG(void);
	~RFG(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);
	virtual void set_default();
protected:
  	void compose_request(const std::string &buf);
// 	void handle_read_check_response();
};



#endif