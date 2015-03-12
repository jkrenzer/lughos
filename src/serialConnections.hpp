#ifndef SERIAL_CONNECTIONS_HPP
#define SERIAL_CONNECTIONS_HPP

#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp>

#include <string>
#include <vector>
#include "asioConnections.hpp"
#include <boost/thread/recursive_mutex.hpp>

namespace lughos
{

typedef boost::shared_ptr<boost::asio::serial_port> serial_port_ptr;

#define SERIAL_PORT_READ_BUF_SIZE 256
enum flow_constroll_bit {off, software, hardware};
enum parity_bit { none, odd, even };
enum stop_bits_num { one, onepointfive, two };

/**
* @class serialContext
* @brief context for serial connection, contains hardwarebit definition
*
*/
class serialContext
{
public:

  typedef boost::shared_ptr<boost::asio::serial_port> SocketPointer;

  boost::logic::tribool DCD;
  boost::logic::tribool DTR;
  boost::logic::tribool DSR;
  boost::logic::tribool RTS;
  boost::logic::tribool CTS;
  boost::logic::tribool RI;

  serialContext() : DCD ( boost::logic::indeterminate ),  DTR ( boost::logic::indeterminate ), DSR ( boost::logic::indeterminate ),  RTS ( boost::logic::indeterminate ), CTS ( boost::logic::indeterminate ), RI ( boost::logic::indeterminate )
  {

  }
};

/**
* @class serialConnection
* @brief class for a serial connection
*
  */
class serialConnection : public asioConnection<serialContext>
{
private:
  serialConnection ( const serialConnection &p );
  serialConnection &operator= ( const serialConnection &p );
  /**
  * @brief waits for callback
  *
  * @param port_ boost::asio::serial_port& for callback
  * @param error ...
  * @return void
  */

public:
  serialConnection ( boost::shared_ptr<boost::asio::io_service> io_service ) ;

  serialConnection();

  ~serialConnection ( void );
  /**
  * @brief Set end-of-line-character
  *
  * @param c New char as EOL-char
  * @return void
  */



  void set_port ( std::string port );
  void reset();

  /**
  * @brief sets baud rate, please check device manual
  *
  * @param baud_rate integer for boud rate
  * @return void
  */
  void set_baud_rate ( const int baud_rate );
  /**
  * @brief sets charakter size of the request, please check device manual
  *
  * @param character_size int, number of charakters
  * @return void
  */
  void set_character_size ( const int character_size );
  /**
  * @brief sets flow controll of the request one of {off, software, hardware}, please check device manual
  *
  * @param controll_type flow_constroll_bit {off, software, hardware}
  * @return void
  */
  void set_flow_controll ( flow_constroll_bit controll_type );
  /**
  * @brief parity type of the request one of { none, odd, even }, please check device manual
  *
  * @param parity_type one of { none, odd, even }
  * @return void
  */
  void set_parity ( parity_bit parity_type );
  /**
  * @brief number of stop bits. one of { one, onepointfive, two }, please check device manual
  *
  * @param stop { one, onepointfive, two }
  * @return void
  */
  void set_stop_bits ( stop_bits_num stop );
  std::string port_name;
  /**
  * @brief sets default parameters
  *
  * @return void
  */
  virtual void initialize();
  
  virtual void connect(boost::function<void(void)> callback);

  boost::asio::serial_port_base::flow_control flow_control;
  boost::asio::serial_port_base::character_size character_size;
  boost::asio::serial_port_base::baud_rate baud_rate;
  boost::asio::serial_port_base::parity parity;
  boost::asio::serial_port_base::stop_bits stop_bits;

};


}                                                           // namespace lughos
#endif
