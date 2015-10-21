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

#ifndef TCP_CONNECTIONS_HPP
#define TCP_CONNECTIONS_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp>

#include <string>
#include <vector>
#include <cstring>
#include "asioConnections.hpp"

#include <iostream>
#include <boost/array.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>



using boost::asio::ip::tcp;

namespace lughos
{

/**
* @class tcpContext
* @brief context for tcp connection, contains hardwarebit definition
*
*/
class tcpContext
{
public:
  typedef boost::shared_ptr<tcp::socket> SocketPointer;
};
/**
* @class tcpConnection
* @brief class for a tcp connection
*
  */
class tcpConnection: public asioConnection<tcpContext>
{
private:
  tcpConnection ( const tcpConnection &p );
  tcpConnection &operator= ( const tcpConnection &p );

protected:
  boost::shared_ptr<tcp::resolver> resolver;
  boost::shared_ptr<tcp::resolver::query> query;
  boost::shared_ptr<tcp::endpoint> endpoint;

  std::string server;

  virtual void initialize();
  virtual void shutdown();

  void handle_resolve ( boost::function<void(const boost::system::error_code& err) > callback, const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator );
  void handle_connect ( boost::function<void(const boost::system::error_code& err) > callback, const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator = tcp::resolver::iterator() );
  virtual void connect ( boost::function<void ( const boost::system::error_code& err ) > callback = boost::function<void ( const boost::system::error_code& err ) >() );
  virtual void disconnect();


public:

  tcpConnection();
  virtual ~tcpConnection ( void );

  std::string server_name;
  bool IPv6;
  void set_port ( std::string port );

  std::string port_name;

};

}                                                           // namespace lughos

#endif
