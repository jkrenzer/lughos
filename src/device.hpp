#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "connectionImpl.hpp"
#include "basicObject.hpp"

#define GUARD boost::lock_guard<boost::recursive_mutex> guard(mutex);

namespace lughos
{
  
  typedef boost::recursive_mutex Mutex;
  
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
      return this->interpretAnswer(connection->read());
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
    
    bool connect(ConnectionImpl* connection)
    {
      GUARD
      this->connection = boost::shared_ptr<ConnectionImpl>(connection);
      this->connected = connection->testconnection();
      this->init();
      return this->connected;
    }
    
    bool isConnected()
    {
      GUARD
      return this->connected;
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
      return this->inputOutputImplementation(query);
    }
    
    DeviceImpl() : connection()
    {
      
    }
    
    ~DeviceImpl()
    {
      
    }
    
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