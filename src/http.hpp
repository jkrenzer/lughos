/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

#ifndef TCP_ASYNC_HPP
#define TCP_ASYNC_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

#include <string>
#include <vector>
#include <cstring>

#include "connectionImpl.hpp"
#include "tcpConnections.hpp"
#include <boost/regex.hpp>

#include <iostream>
#include <boost/array.hpp>
#include "Dict.hpp"



using boost::asio::ip::tcp;

class tcpAsync : virtual public Connection<tcpContext>
{
  private:
	tcpAsync(const tcpAsync &p);
	tcpAsync &operator=(const tcpAsync &p); 
	bool writeonly=false;

// 	tcp::socket socket;


  protected:
    
//     	boost::asio::io_service io_service_;
	void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
	virtual void handle_write_request(const boost::system::error_code& err);
	void handle_write_only_request(const boost::system::error_code& err);
	void handle_read_status_line(const boost::system::error_code& err);
	void handle_read_headers(const boost::system::error_code& err);
	void handle_read_content(const boost::system::error_code& err);


	

	
  public:
	tcpAsync(boost::shared_ptr<boost::asio::io_service> io_service);
	~tcpAsync(void);
	
	int write(std::string query);
	int write_only(std::string query);
	
	void abort();
  
};



#endif