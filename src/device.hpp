#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include "connectionImpl.hpp"

#define GUARD boost::lock_guard guard(mutex);

namespace lughos
{
  
  typedef boost::recursive_mutex Mutex;
  
  /**
   * @brief Template-class which deklares the basic interface of a device
   * 
   */
  class DeviceImpl 
  {
  protected:
    Mutex mutex;
    boost::smart_ptr<ConnectionImpl> connection;
    bool initialized;
    bool connected;
    
    virtual void initImplementation() = 0;
    
    virtual void shutdownImplementation() = 0;
    
    virtual std::string composeRequest(std::string query) = 0;
    
    virtual std::string interpretAnswer(std::string query) = 0;
    
    virtual std::string inputOutputImplementation(std::string query)
    {
      connection.write(this->composeRequest(query));
      return this->interpretAnswer(connection.read());
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
      this->initImplementation;
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
      this->shutdownImplementation;
      this->initialized = false;
    }
    
    void connect(ConnectionImpl* connection)
    {
      GUARD
      this->connection = boost::shared_ptr<ConnectionImpl>(connection);
      this->connected = connection->testconnection();
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
      if(this->connection)
	this->connection.reset();
      this->connected = false;
    }
    
    std::string inputOutput(std::string query)
    {
      GUARD
      return this->inputOutputImplementation(query);
    }
    
    DeviceImpl<T>() : connection()
    {
      this->init();
    }
    
    ~DeviceImpl<T>()
    {
      this->shutdown();
    }
    
  };
  
  class Device : public DeviceImpl
  {
    
  };

} //namespace lughos
#endif