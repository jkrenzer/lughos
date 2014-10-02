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
#include "serialSync.hpp"
#include "serialAsync.hpp"

#include <iostream>
#include <boost/array.hpp>
// #include "Dict.hpp"


class PSANetzteil :virtual public serialSync, virtual public serialAsync
{
  private:
	PSANetzteil(const PSANetzteil &p);
	PSANetzteil &operator=(const PSANetzteil &p);
	
	
  public:
	PSANetzteil(void);
	~PSANetzteil(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);
	virtual void set_default();
protected:
  	void compose_request(const std::string &buf);
// 	void handle_read_check_response();
};



#endif