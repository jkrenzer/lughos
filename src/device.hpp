#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <ostream>


#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/regex.hpp>

#include "connectionImpl.hpp"
#include "basicObject.hpp"
#include "threadSafety.hpp"
#include "errorHandling.hpp"


namespace lughos
{
    
  /**
   * @brief Template-class which deklares the basic interface of a device
   * 
   */
  class DeviceImpl : public basicObject
  {
  protected:
    Mutex mutex;

    boost::shared_ptr<ConnectionImpl> connection;
    bool initialized;
    bool connected;
    
    virtual void initImplementation() = 0;
    
    virtual void shutdownImplementation() = 0;
    
    virtual std::string composeRequest(std::string query) = 0;
    
    virtual std::string interpretAnswer(std::string query) = 0;
    
    virtual std::string inputOutputImplementation(std::string query)
    {
      connection->write(this->composeRequest(query));
      connection->waitForCompletion();
      return this->interpretAnswer(connection->read());
    }
    
    virtual std::string inputOutputImplementation(std::string query, boost::regex regExpr)
    {
      connection->write(this->composeRequest(query), regExpr);
      connection->waitForCompletion();
      return this->interpretAnswer(connection->read());
    }
    
    virtual void inputImplementation(std::string query)
    {
      connection->write_only(this->composeRequest(query));
      return;
    }
  public:
    
    /**
     * @brief Initialize device
     * 
     * This method is called on construction time and should initialize the internal states of the device class. 
     * 
     * @return void
     */
    void init()
    {
      GUARD
      this->initImplementation();
      this->initialized = true;
    }
    
    /**
     * @brief Shutdown device
     * 
     * This member-function deactivates the device.
     * 
     * @return void
     */
    void shutdown()
    {
      GUARD
      this->shutdownImplementation();
      this->initialized = false;
    }
    
    bool connect(boost::shared_ptr<ConnectionImpl> connection)
    {
      GUARD
      this->connection = boost::shared_ptr<ConnectionImpl>(connection);
      this->connected = this->connection->testconnection();
      if(this->connected)
	this->init();
      return this->connected;
    }
    
    bool isConnected()
    {
      GUARD
      bool currentlyConnected = this->connection->testconnection();
      if(currentlyConnected && !this->connected)
	this->init();
      else if (!currentlyConnected && this->connected)
	this->initialized = false;
      this->connected = currentlyConnected;
      return currentlyConnected;
    }
    
    bool isInitialized()
    {
      GUARD
      return this->initialized;
    }
    
    void disconnect()
    {
      this->shutdown();
      if(this->connection)
	this->connection.reset();
      this->connected = false;
    }
    
    const ConnectionImpl* const getConnection()
    {
      return this->connection.get();
    }
    
    std::string inputOutput(std::string query)
    {
      GUARD
      if(this->connected)
	return this->inputOutputImplementation(query);
      else
	BOOST_THROW_EXCEPTION( exception() << errorName(std::string("inputOutput_without_connection")) << errorTitle("InputOutput was tried without active connection to device.") << errorSeverity(severity::ShouldNot) );
    }
    
    std::string inputOutput(std::string query, boost::regex regExpr)
    {
      GUARD
      if(this->connected)
	return this->inputOutputImplementation(query,regExpr);
      else
	BOOST_THROW_EXCEPTION( exception() << errorName(std::string("inputOutput_without_connection")) << errorTitle("InputOutput was tried without active connection to device.") << errorSeverity(severity::ShouldNot) );
    }
    
   void input(std::string query)
    {
      GUARD
      if(this->connected)
	this->inputImplementation(query);
      else
	BOOST_THROW_EXCEPTION( exception() << errorName(std::string("input_without_connection")) << errorTitle("Input was tried without active connection to device.") << errorSeverity(severity::ShouldNot) );
    }
    
    DeviceImpl() : connection()
    {
      
    }
    
//     ~DeviceImpl()
//     {
//       
//     }
//     
  };
  
  class Device : public DeviceImpl
  {
  public:
    
    Device()
    {
    }
    
    ~Device()
    {
    }
    
  };

} //namespace lughos
#endif