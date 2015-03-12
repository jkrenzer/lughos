#include "asioConnections.hpp"
#include "log.hpp"

using namespace lughos;

template <class C> asioConnection<C>::asioConnection() : request(), response()
{
  if ( this->endOfLineRegExpr_== boost::regex ( "\r" ) )  lughos::debugLog ( "End of line char: CR" );
  else if ( this->endOfLineRegExpr_==boost::regex ( "\n" ) ) lughos::debugLog( "End of line char: NL" );
  else lughos::debugLog ( "End of line char: " + this->endOfLineRegExpr_ );
  this->isConnected = false;
  this->isInitialized = false;
}

template <class C> asioConnection<C>::~asioConnection ( void )
{
  this->shutdown();
}



template <class C> void asioConnection<C>::execute ( boost::shared_ptr<Query> query )
{
  std::ostream request_stream ( &request );
  request_stream<<query->getQuestion();
  if ( query->getEOLPattern().empty() )
    query->setEOLPattern ( endOfLineRegExpr_ );
  boost::system::error_code ec;

  if ( socket.get() == NULL || !socket->is_open() ) this->open();
  if ( socket.get() == NULL || !socket->is_open() ) return;

  boost::asio::async_write ( *socket, request,
                             boost::bind ( &asioConnection<C>::handle_write_request, this, query,
                                 boost::asio::placeholders::error ) );

  lughos::debugLog ( std::string ( "\"" ) +query->getQuestion()+std::string ( "\" written to port." ));

  try
    {
      this->io_service->poll();
    }
  catch ( ... )
    {
      lughos::debugLog ( std::string ( "I/O-Service exception while polling." ) );
    }


  if ( socket.get() == NULL || !socket->is_open() )
    {
      lughos::debugLog ( std::string ( "Socket is closed despite writing?!" ) );
    }

  if ( this->io_service->stopped() )
    {
      lughos::debugLog ( std::string ( "I/O-Service was stopped after or during writing." ) );
    }


  return 1;
}


template <class C> void asioConnection<C>::handle_write_request ( boost::shared_ptr<Query> query, const boost::system::error_code& err )
{

  if ( !err )
    {
      // Read the response status line.
      boost::asio::async_read_until ( *socket, response, query->getEOLPattern(),
                                      boost::bind ( &asioConnection<C>::handle_read_content, this, query,
                                          boost::asio::placeholders::error ) );
      lughos::debugLog ( std::string ( "Reading until \"" ) + query->getEOLPattern().str() );

    }
  else
    {
      lughos::debugLog ( std::string ( "Error while writing twoway. Error: " +err.message() ) );
      this->isConnected = false;
    }
}

template <class C> void asioConnection<C>::handle_read_rest ( const boost::system::error_code& err )
{

  if ( !err )
    {
      // Start reading remaining data until EOF.
      boost::asio::async_read ( *socket, response,
                                boost::asio::transfer_at_least ( 1 ),
                                boost::bind ( &asioConnection<C>::handle_read_rest, this,
                                              boost::asio::placeholders::error ) );


    }
  else if ( err == boost::asio::error::eof )
    {
      return;
    }
  else
    {
      std::cout << "Error: " << err << "\n";
    }
}

template <class C> void asioConnection<C>::handle_read_content ( boost::shared_ptr<Query> query, const boost::system::error_code& err )
{
//   	this->timeoutTimer.cancel();
  if ( !err )
    {
      // Write all of the data that has been read so far.
      this->handle_read_rest ( err );
      response_string_stream.str ( std::string ( "" ) );
      response_string_stream<< &response;
      lughos::debugLog ( std::string ( "Read \"" ) + response_string_stream.str() + std::string ( "\"." ));
    }
                     else if ( err == boost::asio::error::connection_aborted || err == boost::asio::error::not_connected || err != boost::asio::error::eof || err != boost::asio::error::connection_reset )
    {
      this->isConnected = false;
      this->execute();
    }
  else
    {
      lughos::debugLog ( std::string ( "Unable to read. Got error: " ) +err.message() );
    }

}

template <class C> void asioConnection<C>::wait_callback ( boost::asio::serial_port& socket, const boost::system::error_code& error )
{
  //std::cout << " Calling wait-handler.";
  if ( error )
    {
      // Data was read and this timeout was canceled
      lughos::debugLog ( std::string ( "Timeout cancelled  because data was read sucessfully." ) );
      return;
    }
  //std::cout << " Timed out.";
  try
    {
      lughos::debugLog ( std::string ( "Timed out while waiting for answer." ));
      this->abort();
    }
  catch ( ... )
    {
      lughos::debugLog ( std::string ( "Exception while timeout and cancelling operation.") );
    }
}

template <class C> void asioConnection<C>::abort()
{
  try
    {
      socket->cancel();
      lughos::debugLog ( std::string ( "Aborting." ));
    }
  catch ( ... )
    {
      lughos::debugLog ( std::string ( "Error while trying to perform requested abort." ));
    }
}

template <class C> bool asioConnection<C>::connected()
{
  return this->isConnected && this->socket->is_open();
}

template <class C> bool asioConnection<C>::initialized ()
{
  return this->initialized();
}


template <class C> bool asioConnection<C>::test()
{
  try
    {
      this->initialize();
      this->shutdown();
      return this->initialized();
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
      if ( socket )
        {
          debugLog ( std::string ( "Stopping connection" ) );
          this->abort();
          socket->close();
          socket.reset();

        }
    }
  catch ( ... )
    {
    }
// 	io_service->stop();
// 	io_service->reset();
}


template <class C> boost::regex asioConnection<C>::endOfLineRegExpr() const
{
  return this->endOfLineRegExpr_;
}

template <class C> void asioConnection<C>::endOfLineRegExpr ( boost::regex c )
{
  this->endOfLineRegExpr_ = c;
}


