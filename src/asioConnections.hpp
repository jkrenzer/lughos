#ifndef ASIO_CONNECTIONS_HPP
#define ASIO_CONNECTIONS_HPP

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

#include <iostream>
#include <boost/array.hpp>

namespace lughos
{

/**
* @class asioConnection
* @brief Class describing asynchronous IO
*
*/
template <class C>
class asioConnection : virtual public Connection<C>
{
private:


  /**
    * @brief Copy Constructor
    *
    * @param p
    */

  asioConnection ( const asioConnection &p );

  /**
  * @brief Asignment operator
  *
  * @param p ...
  * @return asioConnection&
  */

  asioConnection &operator= ( const asioConnection &p );

  /**
  * @brief Wait-callback
  * Called at timeout.
  *
  * @param port_ ...
  * @param error ...
  * @return void
  */

  void wait_callback ( boost::asio::serial_port& port_, const boost::system::error_code& error );

protected:

  typedef typename C::SocketPointer SocketPointer;

  SocketPointer socket;

  void handle_write_request ( boost::shared_ptr<Query> query, const boost::system::error_code& err );

  /**
  * @brief Callback for reading answer from port
  *
  * @param err ...
  * @return void
  */

  void handle_read_content ( boost::shared_ptr<Query> query,const boost::system::error_code& err );
  
  void handle_read_rest (const boost::system::error_code& err );




public:


  /**
  * @brief Constructor
  *
  * @param io_service ...
  */

  asioConnection() ;
  /**
    * @brief Destructor
    *
    * @param  ...
    */


  ~asioConnection ( void );

  /**
    * @brief Execute query on connection.
    *
    * @param query ...
    * @return void
    */
  void execute ( boost::shared_ptr<Query> query );

  /**
    * @brief Abort all action on connection
    *
    * @param query ...
    * @return void
    */

  virtual bool test();

  virtual bool initialized();

  virtual bool connected();

  virtual void shutdown();

  virtual void abort();

  void endOfLineRegExpr ( boost::regex c );

  boost::regex endOfLineRegExpr() const;

protected:
  std::deque<char> write_msgs_;
  boost::regex endOfLineRegExpr_;
  boost::asio::streambuf response;
  boost::asio::streambuf request;
  bool isConnected;
  bool isInitialized;


  std::ostringstream response_string_stream;
};

}                                                           // namespace lughos

#endif
