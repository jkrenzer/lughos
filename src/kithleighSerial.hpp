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

#include <iostream>
#include <boost/array.hpp>
#include "Dict.hpp"


class kithleighSerial :virtual public serialSync
{
  private:
	kithleighSerial(const kithleighSerial &p);
	kithleighSerial &operator=(const kithleighSerial &p);
	
	
  public:
	kithleighSerial(void);
	~kithleighSerial(void);
protected:
  	void compose_request(const std::string &buf);
// 	void handle_read_check_response();
};



#endif