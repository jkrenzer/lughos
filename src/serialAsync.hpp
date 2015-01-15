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
    
    /**
     * @brief Callback for write-only operations
     * 
     * @param err ...
     * @return void
     */
    
    virtual void handle_write_only(const boost::system::error_code& err);
    
    /**
     * @brief Function for writing data to port
     * 
     * @param buf ...
     * @param size ...
     * @return int
     */
    
    int write_some(const char *buf, const int &size);
    
    /**
      * @brief Function to write data to port asynchonously
      * 
      * @param buf ...
      * @param size ...
      * @return int
      */
    
    
    int write_some_async(const char *buf, const int &size);
        
    /**
      * @brief Callback for read-write-request
      * 
      * @param err ...
      * @return void
      */
    
    void handle_write_request(const boost::system::error_code& err);
    
    /**
      * @brief Callback for reading answer from port
      * 
      * @param err ...
      * @return void
      */
    
    void handle_read_content(const boost::system::error_code& err);


	
  public:
	serialAsync(boost::shared_ptr<boost::asio::io_service> io_service) ;
	~serialAsync(void);
	
	int write(std::string query);
	int write_only(std::string query);
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