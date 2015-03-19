#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <ostream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <boost/asio/io_service.hpp>

#include "connectionImpl.hpp"
#include "BasicObject.hpp"
#include "threadSafety.hpp"
#include "errorHandling.hpp"


namespace lughos
{
    
  /**
   * @brief Template-class which deklares the basic interface of a device
   * 
   */
  class DeviceImpl : public BasicObject
  {
  protected:
    
    Mutex mutex;
    
    boost::shared_ptr<boost::asio::io_service> ioService;
    boost::shared_ptr<boost::asio::io_service::work> ioServiceWork;

    boost::shared_ptr<ConnectionImpl> connection;
    bool initialized;
    bool connected;
    
    virtual void initImplementation() = 0;
    
    virtual bool isConnectedImplementation() = 0;
    
    virtual void shutdownImplementation() = 0;
    
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
      this->initImplementation();
      ExclusiveLock lock(this->mutex);
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
      this->shutdownImplementation();
      ExclusiveLock lock(this->mutex);
      this->initialized = false;
    }
    
    bool connect(boost::shared_ptr<ConnectionImpl> connection)
    {
      {
	ExclusiveLock lock(this->mutex);
	this->connection = boost::shared_ptr<ConnectionImpl>(connection);
	this->connection->ioService(this->ioService);
	this->connected = this->connection->test();
      }
      SharedLock lock(this->mutex);
      if(this->connected)
      {
	lock.unlock();
	this->init();
	lock.lock();
      }
      return this->connected;
    }
    
    bool isConnected()
    {
      bool currentlyConnected = this->connection->test();
      UpgradeLock lock(this->mutex);
      if (!currentlyConnected && this->connected)
      {
	upgradeLockToExclusive llock(lock);
	this->connected = false;
      }
      {
	lock.unlock();
	bool isConnected = this->isConnectedImplementation();
	lock.lock();
	upgradeLockToExclusive llock(lock);
	this->connected = currentlyConnected ? isConnected  : false;
      }
      return this->connected;
    }
    
    bool isInitialized()
    {
      SharedLock lock(this->mutex);
      return this->initialized;
    }
    
    void disconnect()
    {
      this->shutdown();
      ExclusiveLock lock(this->mutex);
      if(this->connection)
	this->connection.reset();
      this->connected = false;
    }
    
    const ConnectionImpl* const getConnection()
    {
      SharedLock lock(this->mutex);
      return this->connection.get();
    }
    
    std::string inputOutput(std::string query, boost::regex regExpr = boost::regex())
    {
      SharedLock lock(this->mutex);
      if(this->connected)
      {
	boost::shared_ptr<Query> q(new Query(query));
	if(!regExpr.empty())
	  q->setEOLPattern(regExpr);
	this->connection->execute(q);
	try
	{
	  return q->getAnswer();
	}
	catch(...)
	{
	  return std::string("");
	}
      }
      else
 	BOOST_THROW_EXCEPTION( exception() << errorName(std::string("inputOutput_without_connection")) << errorTitle("InputOutput was tried without active connection to device.") << errorSeverity(severity::ShouldNot) );
        return std::string("");
    }
    
    void input(std::string query, boost::regex regExpr = boost::regex())
    {
      SharedLock lock(this->mutex);
      if(this->connected)
      {
	boost::shared_ptr<Query> q(new Query(query));
	if(!regExpr.empty())
	  q->setEOLPattern(regExpr);
	this->connection->execute(q);
	return;
      }
      else
	BOOST_THROW_EXCEPTION( exception() << errorName(std::string("input_without_connection")) << errorTitle("Input was tried without active connection to device.") << errorSeverity(severity::ShouldNot) );
    }
    
    boost::shared_ptr<boost::asio::io_service> getIoService()
    {
      return this->ioService;
    }
    
    DeviceImpl() : connection(), ioService(new boost::asio::io_service), ioServiceWork(new boost::asio::io_service::work(*ioService))
    {
      boost::thread thread(boost::bind(&boost::asio::io_service::run, this->ioService));
    }
    
    virtual ~DeviceImpl()
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