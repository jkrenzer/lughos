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

  void initialize();
  void shutdown();

  void handle_resolve ( boost::function<void() > callback, const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator );
  void handle_connect ( boost::function<void() > callback, const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator = tcp::resolver::iterator() );
  void connect ( boost::function<void ( void ) > callback = boost::function<void ( void ) >() );
  void disconnect();


public:

  tcpConnection();
  ~tcpConnection ( void );

  std::string server_name;
  bool IPv6;
  void set_port ( std::string port );

  std::string port_name;

};

}                                                           // namespace lughos

#endif
