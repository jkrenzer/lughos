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

}

serialConnection::serialConnection() : flow_control(), baud_rate(), character_size()
{
  this->endOfLineRegExpr_ = boost::regex ( "\n" );
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
  this->isInitialized = false;
  lughos::debugLog ( std::string ( "initializing serial connection on port" ) + this->port_name );
  if ( port_name.empty() )
    {
      lughos::debugLog ( std::string ( "Serial connection not initialized. No port-name set." ) );
      shutdown();
      return;
    }

  if ( !io_service )
    {
      lughos::debugLog ( std::string ( "Serial connection not initialized. No valid io_service." ) );
      shutdown();
      return;
    }

  boost::system::error_code ec;

  socket.reset ( new boost::asio::serial_port ( *io_service ) );

  /* As windows is volatile with it's serial ports we have to be prepared for anything. So try and catch as if running for your life! */

  try
    {
      socket->open ( port_name.c_str(), ec ); //Keep your fingers crossed...
      if ( ec ) // Boost gave us an error-message
        {
          lughos::debugLog ( std::string ( "error : socket->open() failed on port " ) + port_name.c_str() + std::string ( ", with error:" ) + ec.message().c_str() );
          shutdown();
          return;
        }
    }
  catch ( ... )
    {
      // Unfortunatle we got an arbitrary system-exception. :/
      lughos::debugLog ( std::string ( "error : socket->open() failed on port " ) + port_name.c_str() + std::string ( " with an unknown exception." ) );
      shutdown();
      return;
    }

// 	this->reset();
  // option settings...


  try
    {
      socket->set_option ( boost::asio::serial_port_base::baud_rate ( baud_rate ) );
    }
  catch ( ... )
    {
// 	      ofs << "baud rate problems" << std::endl;
    }

  try
    {
      socket->set_option ( boost::asio::serial_port_base::character_size ( character_size.value() ) );
    }
  catch ( ... )
    {
// 	      ofs << "character_size problems" << std::endl;
    }

  try
    {
      int i=0;
      socket->set_option ( stop_bits );
    }
  catch ( ... )
    {
// 	      ofs << "stop_bits problems" << std::endl;
    }


  try
    {
      int i=0;
      socket->set_option ( boost::asio::serial_port_base::parity ( parity ) );
    }
  catch ( ... )
    {
// 	      ofs << "parity problems" << std::endl;
    }

  try
    {
      socket->set_option ( boost::asio::serial_port_base::flow_control ( flow_control ) );
    }
  catch ( ... )
    {
// 	      ofs << "flow_control problems" << std::endl;
    }

// 	ofs << "start is fine" << std::endl;
// 	ofs.flush();
// 	/*/*/*/*ofs*/*/*/*/.close();
// 	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
//   io_service->run();
  this->isInitialized = true;
  return;


}

void serialConnection::connect ( boost::function<void() > callback )
{
  this->isConnected = false;
  try
    {
      if (this->socket)
      {
        debugLog(std::string("Trying to connect to port ") + this->port_name);
        this->socket->open ( this->port_name );
      }
      else
      {
        return;
      }
    }
  catch ( ... )
    {
      return;
    }
    if (this->socket->is_open())
    {
      this->isConnected = true;
      debugLog(std::string("Port ") + this->port_name + std::string(" sucessfully opened."));
    }
    else
      debugLog(std::string("Port ") + this->port_name + std::string(" did not open up."));
    
  if ( callback && this->isConnected )
    {
      debugLog(std::string("Calling callback function."));
      callback();
    }
  return;
}

void serialConnection::reset()
{
  this->abort();
  this->shutdown();
  this->initialize();
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

}

void serialConnection::set_baud_rate ( const int baud )
{

  baud_rate=boost::asio::serial_port_base::baud_rate ( baud );

}
void serialConnection::set_character_size ( const int size )
{

  character_size=boost::asio::serial_port_base::character_size ( size );

}

void serialConnection::set_flow_controll ( flow_constroll_bit controll_type )
{

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
