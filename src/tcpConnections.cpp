#include <iostream>
#include <fstream>

#include "tcpConnections.hpp"
#include "log.hpp"

#ifdef WIN32
#include <windows.h>
#include <winioctl.h>
#endif

using namespace lughos;

tcpConnection::tcpConnection() : endpoint(new tcp::endpoint())
{
this->isConnected = false;
this->endOfLineRegExpr_ = boost::regex ( "\n" );
}

tcpConnection::~tcpConnection(void)
{
// 	stop();
}

void tcpConnection::initialize()
{
  ExclusiveLock lock(this->mutex);
  this->isConnected = false;
  this->isInitialized = false;
    if (server_name.empty()||port_name.empty()) {
		lughos::debugLog("Connection not initialized. Please set server/port name!");
		return;
	}
    lughos::debugLog("Initializing TCP-connection to" + server_name + ":" + port_name);
    resolver.reset(new tcp::resolver(*this->io_service));
    query.reset(new tcp::resolver::query(server_name, port_name));
    socket.reset(new boost::asio::ip::tcp::socket(*this->io_service));
    endpoint.reset(new tcp::endpoint());
//     boost::asio::socket_base::keep_alive keepAlive(true);
//     socket->set_option(keepAlive); //Seems to be only allowed after connect :/
    this->isInitialized = true;
}

void tcpConnection::set_port(std::string port)
{
  ExclusiveLock lock(this->mutex);
  port_name=port;
}

void tcpConnection::connect(boost::function<void(void)> callback)
{
  ExclusiveLock lock(this->mutex);
  this->socket.reset(new tcp::socket(*io_service));
  if (!this->endpoint->address().is_unspecified())
  {
    socket->async_connect(*this->endpoint,
        boost::bind(&tcpConnection::handle_connect, this, callback,
          boost::asio::placeholders::error, tcp::resolver::iterator()));
    lughos::debugLog(std::string("Connecting to server ")+server_name);
  }
  else
  {
    resolver->async_resolve(*this->query, boost::bind(&tcpConnection::handle_resolve, this, callback,
          boost::asio::placeholders::error, boost::asio::placeholders::iterator));
    lughos::debugLog(std::string("Trying to connect to ") + server_name);
  }
}

void tcpConnection::disconnect()
{
  this->abort();
}

void tcpConnection::handle_resolve(boost::function<void()> callback, const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{

  if (!err)
  {
    ExclusiveLock lock(this->mutex);
    *this->endpoint = *endpoint_iterator;
    socket->async_connect(*this->endpoint,
        boost::bind(&tcpConnection::handle_connect, this, callback,
          boost::asio::placeholders::error, ++endpoint_iterator));
    lughos::debugLog(std::string("Resolved address of server ")+server_name);
  }
  else
  {
    lughos::debugLog(std::string("Unable to resolve address of server ")+server_name+std::string(". Got error: ")+err.message());
  }

}

void tcpConnection::handle_connect(boost::function<void (void)> callback, const boost::system::error_code& err,
      tcp::resolver::iterator endpoint_iterator)
{ 
  if(!err)
  {
    ExclusiveLock lock(this->mutex);
    this->isConnected = true;
    lock.unlock();
    lughos::debugLog(std::string("Connected successfully to ")+server_name);
    if(callback)
      callback();
    return;
  }
  if (endpoint_iterator != tcp::resolver::iterator())
  {
    // The connection failed. Try the next endpoint in the list.
    ExclusiveLock lock(this->mutex);
    socket.reset(new boost::asio::ip::tcp::socket(*this->io_service));
    *this->endpoint = *endpoint_iterator;
    socket->async_connect(*this->endpoint,
        boost::bind(&tcpConnection::handle_connect, this, callback,
          boost::asio::placeholders::error, ++endpoint_iterator));
    lughos::debugLog(std::string("Connection failed, trying next possible resolve of ")+server_name);
  }
  else
  {
    ExclusiveLock lock(this->mutex);
    lughos::debugLog(std::string("Unable to connect to server ")+server_name+std::string(". Got error: ")+err.message());
    this->endpoint.reset(new tcp::endpoint);
    return;
  }
}

void tcpConnection::shutdown()
{
  this->abort();
  ExclusiveLock lock(this->mutex);
  this->socket->close();
  this->socket.reset();
  this->query.reset();
  this->resolver.reset();
  this->endpoint.reset();
  this->isInitialized = false;
  return;
}