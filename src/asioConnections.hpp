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
  
  std::list<boost::shared_ptr<Query>> queryStash;
  
  void stashQuery(boost::shared_ptr<Query> query)
  {
    ExclusiveLock lock(mutex);
    this->queryStash.push_back(query);
    this->queryStash.unique();
  }
  
  void unstashQuery(boost::shared_ptr<Query> query)
  {
    ExclusiveLock lock(mutex);
    auto it = std::find_if(queryStash.begin(), queryStash.end(), [&](boost::shared_ptr<Query> const& p) {
    return p == query;});
    if(it != queryStash.end())
      this->queryStash.erase(it);
  }

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
  SharedLock lock(mutex);
  if ( this->endOfLineRegExpr_== boost::regex ( "\r" ) )  LUGHOS_LOG(log::SeverityLevel::informative) <<  ( "End of line char: CR" );
  else if ( this->endOfLineRegExpr_==boost::regex ( "\n" ) ) LUGHOS_LOG(log::SeverityLevel::informative) << ( "End of line char: NL" ) ;
  else LUGHOS_LOG(log::SeverityLevel::informative) <<  ( "End of line char: " + this->endOfLineRegExpr_.str() );
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
  LUGHOS_LOG_FUNCTION();
  this->stashQuery(query); //Handlers DO NOT keep an smart_ptr alive!! BEWARE! How about signals, functors...?!
  if (!query)
    return;
  if (err)
  {
    LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Unable to connect for sending. Error: " ) + err.message() );
    query->setError(std::string ( "Unable to connect for sending. Error: " ) + err.message() );
    this->unstashQuery(query);
    return;
  }
  
  if ( query->getEORPattern().empty() )
    query->setEORPattern ( endOfLineRegExpr_ );
  boost::system::error_code ec;
  SharedLock lock(this->mutex);
  if ( !this->initialized())
  {
    lock.unlock();
    LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Initalizing for sending." ) );
    this->initialize();
    lock.lock();
    if (!this->initialized() || !socket)
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Unable to initialize for sending." ) );
      query->setError(std::string ( "Unable to initialize for sending." ));
      lock.unlock();
      this->unstashQuery(query);
      return;
    }
  }
  if (!this->connected())
  {
    lock.unlock();
    LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Connecting for sending." ) );
    this->connect(boost::bind(&asioConnection<C>::execute, this, query, boost::asio::placeholders::error));
    return;
  }
  lock.unlock();
  {
    ExclusiveLock llock(this->mutex);
    this->timeoutTimer->expires_from_now(boost::posix_time::seconds(1));
    this->timeoutTimer->async_wait(this->timingStrand->wrap(boost::bind ( &asioConnection<C>::handle_timeout, this, query,
                                 boost::asio::placeholders::error )));
  }
  lock.lock();
  query->busy();
  boost::asio::async_write ( *socket, query->output(),
                             this->ioStrand->wrap(boost::bind ( &asioConnection<C>::handle_write_request, this, query,
                                 boost::asio::placeholders::error )) );

  LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Sent \"" ) + query->getQuestion() + query->getEOSPattern() + std::string ( "\" from " ) + query->idString);
  lock.unlock();
//   try
//     {
//       this->io_service->poll();
//     }
//   catch ( ... )
//     {
//       LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "I/O-Service exception while polling." ) );
//       query->setError(std::string ( "I/O-Service exception while polling." ));
//       lock.unlock();
//       this->unstashQuery(query);
//       this->abort();
//       return;
//     }


  if ( !socket || !socket->is_open() )
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Socket is closed despite writing?!" ) );
      query->setError(std::string ( "Socket is closed despite writing?!" ));
      lock.unlock();
      this->unstashQuery(query);
      this->abort();
      return;
    }

  if ( this->io_service->stopped() )
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "I/O-Service was stopped after or during writing." ) );
      query->setError(std::string ( "I/O-Service was stopped after or during writing." ));
      lock.unlock();
      this->unstashQuery(query);
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
      this->timeoutTimer->expires_from_now(boost::posix_time::seconds(2));
      this->timeoutTimer->async_wait(this->timingStrand->wrap(boost::bind ( &asioConnection<C>::handle_timeout, this, query,
                                 boost::asio::placeholders::error )));
      boost::asio::async_read_until ( *socket, query->input(), query->getEORPattern(),
                                      this->ioStrand->wrap(boost::bind ( &asioConnection<C>::handle_read_content, this, query,
                                          boost::asio::placeholders::error )) );
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Reading until \"" ) + query->getEORPattern().str() );
      return;
    }
  else
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Error while writing twoway. Error: " +err.message() ) );
      query->setError(std::string ( "Error while writing twoway. Error: " +err.message()));
      
      this->unstashQuery(query);
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
      this->unstashQuery(query);
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Query " ) + query->idString + std::string(" completed successfully."));
      return;
    }
    else if (err == boost::asio::error::operation_aborted)
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Query ")+ query->idString + std::string(" was aborted: " ) + err.message());
      query->setError(err.message());
      this->unstashQuery(query);
      return;
    }
    else if ( err == boost::asio::error::connection_aborted || err == boost::asio::error::not_connected || err == boost::asio::error::connection_reset)
    {
      ExclusiveLock lock(this->mutex);
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Connection lost? Error while reading: " ) + err.message());
      query->reset();
      this->isConnected = false;
      lock.unlock();
      query->retry();
      this->execute(query);
      return;
    }
  else
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Unable to read. Got error: " ) +err.message() );
      query->setError(err.message());
      
      this->unstashQuery(query);
      return;
    }

}

template <class C> void asioConnection<C>::handle_timeout ( boost::shared_ptr<Query> query, const boost::system::error_code& error )
{
  if(!error)
  {
    LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Timed out while waiting for reply of device." ));
    this->abort();
    this->unstashQuery(query);
    query->setError(std::string("Timed out."));           // TODO signal error states in query
    return;
  }
  else if ( error == boost::asio::error::operation_aborted)
  {
    // Data was read and this timeout was canceled
    LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Timeout cancelled for ") + query->idString + std::string("." ) );
    return;
  }
  else
  {
    LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Timeout cancelled because of error. Error-message: " ) + error.message() );
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
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Aborting." ));
    }
  catch ( std::exception& e )
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Error while trying to perform requested abort: ") + e.what());
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
      LUGHOS_LOG(log::SeverityLevel::informative) <<  ( std::string ( "Testing connection." ));
      this->initialize(); //TODO we need to connect, for better or for worse...
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
  LUGHOS_LOG_FUNCTION();
  try
    {
      ExclusiveLock lock(this->mutex);
      this->isInitialized = false;
      this->isConnected = false;
      if ( socket )
        {
          LUGHOS_LOG(log::SeverityLevel::informative) << "Shutting connection down.";
          if (this->socket->is_open())
          {
            lock.unlock();
            this->abort();
            lock.lock();
            socket->close();
          }
          socket.reset();
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
