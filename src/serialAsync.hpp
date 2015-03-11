#ifndef SERIAL_ASYNC_HPP
#define SERIAL_ASYNC_HPP

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
#include "serialConnections.hpp"

#include <iostream>
#include <boost/array.hpp>
// #include "Dict.hpp"



/**
 * @class serialAsync
 * @brief Class describing asynchronous serial connections
 * 
 */

class serialAsync : virtual public Connection<serialContext>
{
  private:
    /**
    * @brief Copy Constructor
    * 
    * @param p 
    */

    serialAsync(const serialAsync &p);
    
    /**
      * @brief Asignment operator
      * 
      * @param p ...
      * @return serialAsync&
      */
    
    serialAsync &operator=(const serialAsync &p); 
    
    /**
      * @brief Wait-callback
      * Called at timeout.
      * 
      * @param port_ ...
      * @param error ...
      * @return void
      */
    
    void wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error);

  protected:
    
    void handle_write_request(boost::regex& regExpr, const boost::system::error_code& err);
    
    /**
      * @brief Callback for reading answer from port
      * 
      * @param err ...
      * @return void
      */
    
    void handle_read_content(boost::regex& regExpr,const boost::system::error_code& err);
    


	
  public:
    
    
    /**
     * @brief Constructor
     * 
     * @param io_service ...
     */
    
      serialAsync() ;
      /**
	* @brief Destructor
	* 
	* @param  ...
	*/
      
      
      ~serialAsync(void);
      
      /**
	* @brief Write to serial port and wait for answer
	* 
	* @param query ...
	* @return int
	*/
      int write(std::string query, boost::regex regExpr);
      
      /**
	* @brief Write to serial port and retur immediatly
	* 
	* @param query ...
	* @return int
	*/
      
      void abort();
      
  protected:
/////////////
	std::deque<char> write_msgs_;
	void do_close(const boost::system::error_code& error);
	void write_complete(const boost::system::error_code& error);
	void write_start(void);
	void do_write(const char msg);
	void read_complete(const boost::system::error_code& error, size_t bytes_transferred);
	void read_start(void);
////////////
  
};



#endif