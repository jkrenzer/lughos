#include <iostream>
#include <fstream>

#include "serialConnections.hpp"
#include "log.hpp"

#ifdef WIN32
#include <windows.h>
#include <winioctl.h>
#endif

using namespace lughos;

serialConnection::serialConnection ( boost::shared_ptr<boost::asio::io_service> io_service ) : flow_control(), baud_rate(), character_size()
{
  this->ioService ( io_service );
  this->endOfLineRegExpr_ = boost::regex ( "\n" );
  boost::asio::serial_port_base::flow_control flow_control(boost::asio::serial_port::flow_control::none);
  boost::asio::serial_port_base::character_size character_size(boost::asio::serial_port::character_size(8));
  boost::asio::serial_port_base::baud_rate baud_rate(boost::asio::serial_port::baud_rate(9600));
  boost::asio::serial_port_base::parity parity(boost::asio::serial_port::parity::none);
  boost::asio::serial_port_base::stop_bits stop_bits(boost::asio::serial_port::stop_bits::one);
}

serialConnection::serialConnection() : flow_control(), baud_rate(), character_size()
{
  this->endOfLineRegExpr_ = boost::regex ( "\n" );
  boost::asio::serial_port_base::flow_control flow_control(boost::asio::serial_port::flow_control::none);
  boost::asio::serial_port_base::character_size character_size(boost::asio::serial_port::character_size(8));
  boost::asio::serial_port_base::baud_rate baud_rate(boost::asio::serial_port::baud_rate(9600));
  boost::asio::serial_port_base::parity parity(boost::asio::serial_port::parity::none);
  boost::asio::serial_port_base::stop_bits stop_bits(boost::asio::serial_port::stop_bits::one);
}


serialConnection::~serialConnection ( void )
{
  shutdown();
}


void serialConnection::initialize()
{
  /*
   	std::cout <<  "### Starting connection! ###" << std::endl;
   	std::cout << "baud_rate: "<<baud_rate.value() << std::endl;
   	std::cout << "character_size: "<<character_size.value() << std::endl;
   	std::cout << "stop_bits: "<<stop_bits.value()<< std::endl;
   	std::cout << "parity: "<<parity.value() << std::endl;
   	std::cout << "flow_control: "<<flow_control.value() << std::endl;
    std::cout<<"eolc: "<< endOfLineRegExpr()<< std::endl;
    std::cout<<"port name: "<< port_name<< std::endl;
    std::cout << "#############################" << std::endl;*/

  ExclusiveLock lock(this->mutex);
  this->isInitialized = true;
  lughos::debugLog ( std::string ( "initializing serial connection on port" ) + this->port_name );
  if ( port_name.empty() )
    {
      lughos::debugLog ( std::string ( "Serial connection not initialized. No port-name set." ) );
      lock.unlock();
      shutdown();
      return;
    }

  if ( !io_service )
    {
      lughos::debugLog ( std::string ( "Serial connection not initialized. No valid io_service." ) );
      lock.unlock();
      shutdown();
      return;
    }

  socket.reset ( new boost::asio::serial_port ( *io_service ) );
  boost::system::error_code ec;
  this->socket->open( this->port_name,  ec);
  if (ec)
  {
    debugLog(std::string("Error while trying to open port ") + this->port_name + std::string(" for initialization. Error-message: ") + ec.message());
    this->isInitialized = false;
    return;
  }

  try
    {
      socket->set_option ( boost::asio::serial_port_base::baud_rate ( baud_rate ) );
    }
  catch ( std::exception& e )
    {
	lughos::debugLog ( std::string ( "Could not set baud rate. Error: " ) + e.what() );
    }

  try
    {
      socket->set_option ( boost::asio::serial_port_base::character_size ( character_size.value() ) );
    }
  catch ( std::exception& e )
    {
      lughos::debugLog ( std::string ( "Could not set character size. Error: " ) + e.what() );
    }

  try
    {
      int i=0;
      socket->set_option ( stop_bits );
    }
  catch ( std::exception& e )
    {
      lughos::debugLog ( std::string ( "Could not set stop bits. Error: " ) + e.what() );
    }

  try
    {
      int i=0;
      socket->set_option ( boost::asio::serial_port_base::parity ( parity ) );
    }
  catch ( std::exception& e )
    {
      lughos::debugLog ( std::string ( "Could not set parity. Error: " ) + e.what() );
    }

  try
    {
      socket->set_option ( boost::asio::serial_port_base::flow_control ( flow_control ) );
    }
  catch ( std::exception& e )
    {
      lughos::debugLog ( std::string ( "Could not set flow control. Error: " ) + e.what() );
    }
  return;


}

void serialConnection::connect ( boost::function<void(const boost::system::error_code& err) > callback )
{
  ExclusiveLock lock(this->mutex);
  this->isConnected = false;

  try
    {
      if (this->socket && !this->socket->is_open())
      {
        debugLog(std::string("Trying to connect to port ") + this->port_name);
         boost::system::error_code ec;
        this->socket->open ( this->port_name,  ec);
        if (ec)
        {
          debugLog(std::string("Error while trying to connect to port ") + this->port_name + std::string(" . Error-message: ") + ec.message());
        }
        else
        {
          this->initialize();
        }
      }
      else if (this->socket && this->socket->is_open())
      {
        debugLog(std::string("Already connected to port ") + this->port_name + std::string(". Calling callback function."));
        this->isConnected = true;
        boost::system::error_code ec;
        if(callback)
          callback(ec);
        return;
      }
      else if (!this->socket)
      {
        debugLog(std::string("Port is not initialized,  so wie cannot connect.") + this->port_name);
        this->isInitialized = false;
        return;
      }
      else
      {
        debugLog(std::string("Unable to connect to port ") + this->port_name);
        return;
      }
    }
  catch ( std::exception& e )
    {
      debugLog(std::string("Exception while trying to access port ") + this->port_name + std::string(". Exception-Message: ") + e.what());
      return;
    }
    if (this->socket && this->socket->is_open())
    {
      this->isConnected = true;
      debugLog(std::string("Port ") + this->port_name + std::string(" sucessfully opened."));
    }
    else
      debugLog(std::string("Port ") + this->port_name + std::string(" did not open up."));
    
  if ( callback && this->isConnected )
    {
      debugLog(std::string("Calling callback function."));
      lock.unlock();
      callback(boost::system::error_code());
    }
  return;
}

void serialConnection::reset()
{
  this->abort();
  this->shutdown();
  this->initialize();
  ExclusiveLock lock(this->mutex);
#ifdef WIN32

  DCB dcb;

  HANDLE h_Port = CreateFile ( port_name.c_str(),GENERIC_READ |  GENERIC_WRITE,0,0,OPEN_EXISTING,0,0 );

  GetCommState ( h_Port, &dcb );
//       int pid = *socket->native();
  // play with RTS & DTR
  int iFlags=0;
// 	std::cout << "iFlags="<< iFlags<< std::endl;
//       std::cout << "sockettype="<< porttype << std::endl;
  if ( context.DCD )
    {
      iFlags = 1;
// 	ioctl(pid, TIOCMBIS, &iFlags);
    }
  else if ( !context.DCD )
    {
      iFlags = 0;
// 	ioctl(pid, TIOCMBIC, &iFlags);
    }
  else
    {

    }
  if ( context.DTR )
    {
// 	std::cout << "iFlags DTR="<< iFlags << std::endl;
      dcb.fDtrControl =DTR_CONTROL_ENABLE;
// 	ioctl(pid, TIOCMBIS, &iFlags);
    }
  else if ( !context.DTR )
    {
      dcb.fDtrControl =DTR_CONTROL_DISABLE;
    }
  if ( context.DSR )
    {

    }
  else if ( !context.DSR )
    {

    }
  if ( context.RTS )
    {
      dcb.fRtsControl = RTS_CONTROL_ENABLE;

    }
  else if ( !context.RTS )
    {
      dcb.fRtsControl = RTS_CONTROL_DISABLE;
    }
  if ( context.CTS )
    {

    }
  else if ( !context.CTS )
    {

    }
  if ( context.RI )
    {

    }
  else if ( !context.RI )
    {

    }


#else


  int pid = socket->native();
  // play with RTS & DTR
  int iFlags=0;
// 	std::cout << "iFlags="<< iFlags<< std::endl;
//       std::cout << "sockettype="<< porttype << std::endl;
  if ( context.DCD )
    {
      iFlags = 1;
      ioctl ( pid, TIOCMBIS, &iFlags );
    }
  else if ( !context.DCD )
    {
      iFlags = 0;
      ioctl ( pid, TIOCMBIC, &iFlags );
    }
  else
    {

    }
  if ( context.DTR )
    {
      iFlags = TIOCM_DTR;
// 	std::cout << "iFlags DTR="<< iFlags << std::endl;
      ioctl ( pid, TIOCMBIS, &iFlags );
    }
  else if ( !context.DTR )
    {
      iFlags= TIOCM_DTR;
      ioctl ( pid, TIOCMBIC, &iFlags );
    }
  if ( context.DSR )
    {
      iFlags = TIOCM_DSR;
      ioctl ( pid, TIOCMBIS, &iFlags );
    }
  else if ( !context.DSR )
    {
      iFlags = TIOCM_DSR;
      ioctl ( pid, TIOCMBIC, &iFlags );
    }
  if ( context.RTS )
    {
      iFlags = TIOCM_RTS;
      ioctl ( pid, TIOCMBIS, &iFlags );
    }
  else if ( !context.RTS )
    {
      iFlags = TIOCM_RTS;
// 	std::cout << "iFlags RTS="<< iFlags << std::endl;
      ioctl ( pid, TIOCMBIC, &iFlags );
    }
  if ( context.CTS )
    {
      iFlags = TIOCM_CTS;
      ioctl ( pid, TIOCMBIS, &iFlags );
    }
  else if ( !context.CTS )
    {
      iFlags = TIOCM_CTS;
      ioctl ( pid, TIOCMBIC, &iFlags );
    }
  if ( context.RI )
    {
      iFlags = TIOCM_RI;
      ioctl ( pid, TIOCMBIS, &iFlags );
    }
  else if ( !context.RI )
    {
      iFlags = TIOCM_RI;
      ioctl ( pid, TIOCMBIC, &iFlags );
    }

#endif
}

void serialConnection::set_port ( std::string port )
{
  ExclusiveLock lock(this->mutex);
  this->port_name = port;
}

void serialConnection::set_baud_rate ( const int baud )
{
  ExclusiveLock lock(this->mutex);
  baud_rate=boost::asio::serial_port_base::baud_rate ( baud );
}
void serialConnection::set_character_size ( const int size )
{
  ExclusiveLock lock(this->mutex);
  character_size=boost::asio::serial_port_base::character_size ( size );
}

void serialConnection::set_flow_controll ( flow_constroll_bit controll_type )
{
  ExclusiveLock lock(this->mutex);
  switch ( controll_type )
    {
    case off:  // (can I just type case EASY?)
      flow_control=boost::asio::serial_port_base::flow_control ( boost::asio::serial_port_base::flow_control::none );
      break;

    case software:
      flow_control=boost::asio::serial_port_base::flow_control ( boost::asio::serial_port_base::flow_control::software );
      break;

    case hardware:
      flow_control=boost::asio::serial_port_base::flow_control ( boost::asio::serial_port_base::flow_control::hardware );

    default:
      ;

    }
}

void serialConnection::set_parity ( parity_bit parity_type )
{
  ExclusiveLock lock(this->mutex);
  switch ( parity_type )
    {
    case none:
      parity=boost::asio::serial_port_base::parity ( boost::asio::serial_port_base::parity::none );
      break;

    case odd:
      parity=boost::asio::serial_port_base::parity ( boost::asio::serial_port_base::parity::odd );
      break;

    case even:
      parity=boost::asio::serial_port_base::parity ( boost::asio::serial_port_base::parity::even );

    default:
      ;

    }
}

void serialConnection::set_stop_bits ( stop_bits_num stop_bits_type )
{
  ExclusiveLock lock(this->mutex);
  switch ( stop_bits_type )
    {
    case one:
      stop_bits=boost::asio::serial_port_base::stop_bits ( boost::asio::serial_port_base::stop_bits::one );
      break;

    case onepointfive:
      stop_bits=boost::asio::serial_port_base::stop_bits ( boost::asio::serial_port_base::stop_bits::onepointfive );
      break;

    case two:
      stop_bits=boost::asio::serial_port_base::stop_bits ( boost::asio::serial_port_base::stop_bits::two );

    default:
      ;

    }
}
