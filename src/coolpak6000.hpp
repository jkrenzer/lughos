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


class coolpak6000 :virtual public serialSync, virtual public serialAsync
{
  private:
	coolpak6000(const coolpak6000 &p);
	coolpak6000 &operator=(const coolpak6000 &p);
	

  public:
	coolpak6000(void);
	~coolpak6000(void);
	
	virtual std::string inputoutput(const std::string input, const int async=0);	
protected:
  	void compose_request(const std::string &buf);
// 	void handle_read_check_response();
};



#endif