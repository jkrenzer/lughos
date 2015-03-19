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
#include "log.hpp"
#include "threadSafety.hpp"
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
class asioConnection : public Connection<C>
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

  virtual void handle_timeout ( boost::shared_ptr<Query> query, const boost::system::error_code& error );

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
  
//   void handle_read_rest (const boost::system::error_code& err );




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


  virtual ~asioConnection ( void );

  /**
    * @brief Execute query on connection.
    *
    * @param query ...
    * @return void
    */
  void execute ( boost::shared_ptr<Query> query, const boost::system::error_code& err );
  
  void execute ( boost::shared_ptr<Query> query);

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
  
  virtual void connect(boost::function<void ( const boost::system::error_code& err ) > callback = boost::function<void ( const boost::system::error_code& err ) >()) = 0;

  void endOfLineRegExpr ( boost::regex c );

  boost::regex endOfLineRegExpr() const;

protected:
  std::deque<char> write_msgs_;
  boost::regex endOfLineRegExpr_;
  
  bool isConnected;
  bool isInitialized;

  Mutex mutex;
};

                                                           

/********************************************************** 
 * Template definition in same file as needed by standard *
 **********************************************************/


template <class C> asioConnection<C>::asioConnection()
{
  if ( this->endOfLineRegExpr_== boost::regex ( "\r" ) )  lughos::debugLog ( "End of line char: CR" );
  else if ( this->endOfLineRegExpr_==boost::regex ( "\n" ) ) lughos::debugLog( "End of line char: NL" );
  else lughos::debugLog ( "End of line char: " + this->endOfLineRegExpr_.str() );
  this->isConnected = false;
  this->isInitialized = false;
}

template <class C> asioConnection<C>::~asioConnection ( void )
{
  this->shutdown();
}

template <class C> void asioConnection<C>::execute ( boost::shared_ptr<Query> query)
{
  this->execute(query,boost::system::error_code());
}


template <class C> void asioConnection<C>::execute ( boost::shared_ptr<Query> query, const boost::system::error_code& err  )
{
  if (!query || err)
    return;
  
  if ( query->getEOLPattern().empty() )
    query->setEOLPattern ( endOfLineRegExpr_ );
  boost::system::error_code ec;
  SharedLock lock(this->mutex);
  if ( !this->initialized())
  {
    lock.unlock();
    this->initialize();
    lock.lock();
    if (!this->initialized() || !socket)
    {
      lughos::debugLog ( std::string ( "Unable to initialize for sending." ) );
      query->setError(std::string ( "Unable to initialize for sending." ));
      return;
    }
  }
  if (!this->connected())
  {
    lock.unlock();
    this->connect(boost::bind(&asioConnection<C>::execute, this, query));
    lock.lock();
    if (!this->connected())
    {
      lughos::debugLog ( std::string ( "Unable to connect for sending." ) );
      query->setError(std::string ( "Unable to connect for sending." ));
      return;
    }
  }
  lock.unlock();
  query->busy(this->busy);
  {
    ExclusiveLock llock(this->mutex);
    this->timeoutTimer->expires_from_now(boost::posix_time::seconds(1));
    this->timeoutTimer->async_wait(boost::bind ( &asioConnection<C>::handle_timeout, this, query,
                                 boost::asio::placeholders::error ));
  }
  lock.lock();
  boost::asio::async_write ( *socket, query->output(),
                             boost::bind ( &asioConnection<C>::handle_write_request, this, query,
                                 boost::asio::placeholders::error ) );

  lughos::debugLog ( std::string ( "\"" ) +query->getQuestion()+std::string ( "\" written to port. Query: " ) + query->idString);
  lock.unlock();
  try
    {
      this->io_service->poll();
    }
  catch ( ... )
    {
      lughos::debugLog ( std::string ( "I/O-Service exception while polling." ) );
      query->setError(std::string ( "I/O-Service exception while polling." ));
      this->abort();
      return;
    }


  if ( !socket || !socket->is_open() )
    {
      lughos::debugLog ( std::string ( "Socket is closed despite writing?!" ) );
      query->setError(std::string ( "Socket is closed despite writing?!" ));
      this->abort();
      return;
    }

  if ( this->io_service->stopped() )
    {
      lughos::debugLog ( std::string ( "I/O-Service was stopped after or during writing." ) );
      query->setError(std::string ( "I/O-Service was stopped after or during writing." ));
      this->abort();
      return;
    }


  return;
}


template <class C> void asioConnection<C>::handle_write_request ( boost::shared_ptr<Query> query, const boost::system::error_code& err )
{

  if ( !err )
    {
      // Read the response status line.
      SharedLock lock(this->mutex);
      boost::asio::async_read_until ( *socket, query->input(), query->getEOLPattern(),
                                      boost::bind ( &asioConnection<C>::handle_read_content, this, query,
                                          boost::asio::placeholders::error ) );
      lughos::debugLog ( std::string ( "Reading until \"" ) + query->getEOLPattern().str() );
      return;
    }
  else
    {
      lughos::debugLog ( std::string ( "Error while writing twoway. Error: " +err.message() ) );
      query->setError(std::string ( "Error while writing twoway. Error: " +err.message()));
      ExclusiveLock lock(this->mutex);
      this->isConnected = false;
      this->timeoutTimer->cancel();
    }
}

template <class C> void asioConnection<C>::handle_read_content ( boost::shared_ptr<Query> query, const boost::system::error_code& err )
{
  {
    ExclusiveLock lock(this->mutex);
    this->timeoutTimer->cancel();
  }
  if ( !err )
    {
      // Write all of the data that has been read so far.
      query->ready();
      lughos::debugLog ( std::string ( "Read \"" ) + query->getAnswer() + std::string ( "\" into " ) + query->idString);
      return;
    }
    else if (err == boost::asio::error::operation_aborted)
    {
      lughos::debugLog ( std::string ( "Query ")+ query->idString + std::string(" was aborted: " ) + err.message());
      query->setError(err.message());
      return;
    }
    else if ( err == boost::asio::error::connection_aborted || err == boost::asio::error::not_connected || err != boost::asio::error::eof || err != boost::asio::error::connection_reset)
    {
      ExclusiveLock lock(this->mutex);
      lughos::debugLog ( std::string ( "Connection lost? Error while reading: " ) + err.message());
      query->reset();
      this->isConnected = false;
      lock.unlock();
      this->execute(query);
      return;
    }
  else
    {
      lughos::debugLog ( std::string ( "Unable to read. Got error: " ) +err.message() );
      query->setError(err.message());
      return;
    }

}

template <class C> void asioConnection<C>::handle_timeout ( boost::shared_ptr<Query> query, const boost::system::error_code& error )
{
  if(!error)
  {
    lughos::debugLog ( std::string ( "Timed out while waiting for answer." ));
    this->abort();
    query->setError(std::string("Timed out."));           // TODO signal error states in query
    return;
  }
  else if ( error == boost::asio::error::operation_aborted)
  {
    // Data was read and this timeout was canceled
    lughos::debugLog ( std::string ( "Timeout cancelled for ") + query->idString + std::string("." ) );
    return;
  }
  else
  {
    lughos::debugLog ( std::string ( "Timeout cancelled because of error. Error-message: " ) + error.message() );
    return;
  }
}

template <class C> void asioConnection<C>::abort()
{
  try
    {
      ExclusiveLock lock(this->mutex);
      if (socket)
        socket->cancel();
      lock.unlock();
      lughos::debugLog ( std::string ( "Aborting." ));
    }
  catch ( std::exception& e )
    {
      lughos::debugLog ( std::string ( "Error while trying to perform requested abort: ") + e.what());
    }
}

template <class C> bool asioConnection<C>::connected()
{
  SharedLock lock(this->mutex);
  if (socket)
    return this->isConnected && this->socket->is_open();
  else
    return false;
}

template <class C> bool asioConnection<C>::initialized ()
{
  SharedLock lock(this->mutex);
  return this->isInitialized && this->socket;
}


template <class C> bool asioConnection<C>::test()
{
  try
    {
      this->initialize();
      bool success = this->initialized();
      this->shutdown();
      return success;
    }
  catch ( ... )
    {
      return false;
    }
}


template <class C> void asioConnection<C>::shutdown()
{

  try
    {
      ExclusiveLock lock(this->mutex);
      if ( socket )
        {
          debugLog ( std::string ( "Shutting connection down." ) );
          lock.unlock();
          this->abort();
          lock.lock();
          socket->close();
          socket.reset();
          this->isInitialized = false;
          this->isConnected = false;
          lock.unlock();
        }
    }
  catch ( ... )
    {
    }
//      io_service->stop();
//      io_service->reset();
}


template <class C> boost::regex asioConnection<C>::endOfLineRegExpr() const
{
  SharedLock lock(this->mutex);
  return this->endOfLineRegExpr_;
}

template <class C> void asioConnection<C>::endOfLineRegExpr ( boost::regex c )
{
  ExclusiveLock lock(this->mutex);
  this->endOfLineRegExpr_ = c;
}

} // namespace lughos

#endif
