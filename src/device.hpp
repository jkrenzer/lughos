#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <ostream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include <boost/regex.hpp>
#include <boost/asio/io_service.hpp>
#include "connectionImpl.hpp"
#include "exposedValues.hpp"
#include "exposedClasses.hpp"
#include "exposedMeasuredValues.hpp"
#include "basicTypes.hpp"
#include "threadSafety.hpp"
#include "errorHandling.hpp"


namespace lughos
{
  
  class StateMessage
    {
    public:
      enum Type { Off, Save, Operational, Notice, Warning, Error, Special1 } type_;
    protected:
      std::string name_;
      std::string description_;
      std::string advice_;
    public:
      void advice(std::string advice)
      {
        this->advice_ = advice;
      }
      
      std::string advice()
      {
        return this->advice_;
      }
      
      void description(std::string description)
      {
        this->description_ = description;
      }
      
      std::string description()
      {
        return this->description_;
      }
      
      void name(std::string name)
      {
        this->name_ = name;
      }
      
      std::string name()
      {
        return this->name_;
      }
      
      void type(Type type)
      {
        this->type_ = type;
      }
      
      Type type()
      {
        return this->type_;
      }
      
    };
  
  /**
   * @brief Template-class which deklares the basic interface of a device
   * 
   */
  class DeviceImpl : public ExposedClass
  {
  public:
  
    boost::signals2::signal<void ()> onStateChange;
    boost::signals2::signal<void ()> onConnect;
    boost::signals2::signal<void ()> onDisconnect;
    boost::signals2::signal<void ()> onError;
    
  protected:
    
    Mutex mutex;
    
    boost::shared_ptr<boost::asio::io_service> ioService;
    boost::shared_ptr<boost::asio::io_service::work> ioServiceWork;
    std::vector<boost::shared_ptr<boost::thread> > threadPool;

    boost::shared_ptr<ConnectionImpl> connection;
    bool initialized;
    
    ExposedValue<bool> connected;
    
    std::vector<boost::shared_ptr<StateMessage> > stateMessages;
    
    virtual void initImplementation() = 0;
    
    virtual bool isConnectedImplementation() = 0;
    
    virtual void shutdownImplementation()
    {
    }
    
  public:
    
    /**
     * @brief Initialize device
     * 
     * This method is called as soon as connection to the device is established and should initialize the internal states of the device class. 
     * 
     * @return void
     */
    void init()
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  "Device " << this->name << " is initializing." ;
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
      LUGHOS_LOG(log::SeverityLevel::informative) <<  "Device " << this->name << " is shutting down.";
      this->shutdownImplementation();
      ExclusiveLock lock(this->mutex);
      this->initialized = false;
    }
    
    bool connect(boost::shared_ptr<ConnectionImpl> connection)
    {
      {
	ExclusiveLock lock(this->mutex);
        LUGHOS_LOG(log::SeverityLevel::informative) <<  "Device " << this->name << " has now a connection.";
	this->connection = boost::shared_ptr<ConnectionImpl>(connection);
	this->connection->ioService(this->ioService);
      }
      LUGHOS_LOG(log::SeverityLevel::informative) << "Device " << this->name << " starts connection test.";
     
      return this->isConnected();
    }
    
    bool isConnected()
    {
      std::stringstream tmp;
      if(this->connection)
      {
	bool currentlyConnected = this->connection->test();
	tmp << "Connection test state: " << currentlyConnected << " ";
	UpgradeLock lock(this->mutex);
	if (!currentlyConnected && this->connected)
	{
	  upgradeLockToExclusive llock(lock);
	  this->connected = false;
	}
	{
	  lock.unlock();
	  bool isConnected = this->isConnectedImplementation();
	  tmp << "Device test state: " << isConnected << " ";
	  lock.lock();
	  upgradeLockToExclusive llock(lock);
	  this->connected = currentlyConnected ? isConnected  : false;
	}
	tmp << "Overall state: " << this->connected.getValue();
	LUGHOS_LOG(log::SeverityLevel::informative) << "Device " << this->name << " checked connection. Connection: " << tmp.str() ;
	lock.unlock();
        if(this->connected.getValue())
	  this->init();
	lock.lock();
	return this->connected;
      }
      else 
	return false;
    }
    
    void emitConnectionSignals()
    {
      if(this->connected)
	this->onConnect();
      else
	this->onDisconnect();
    }
    
    bool isInitialized()
    {
      SharedLock lock(this->mutex);
      return this->initialized;
    }
    
    void disconnect()
    {
      LUGHOS_LOG(log::SeverityLevel::informative) <<  "Device " << this->name << " is disconnecting.";
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
    
    template <size_t N>
    std::string inputOutput(char const cstrQuery[N], boost::regex regExpr = boost::regex())
    {
      std::string str(cstrQuery,N);
      return this->inputOutput(str, regExpr);
    }
    
    std::string inputOutput(std::string query, boost::regex regExpr = boost::regex())
    {
      SharedLock lock(this->mutex);
      if(this->connection)
      {
	boost::shared_ptr<Query> q(new Query(query));
	if(!regExpr.empty())
	  q->setEORPattern(regExpr);
	try
	{
	  this->connection->execute(q);
	  std::string answer = q->getAnswer();
	  return answer;
	}
	catch(...)
	{
	  LUGHOS_LOG(log::SeverityLevel::informative) << "Device " << this->name << " got exception while communication over port!" ;
	  this->connected = false;
	  return std::string("");
	}
      }
    }
    
    void input(std::string query, boost::regex regExpr = boost::regex())
    {
      SharedLock lock(this->mutex);
      if(this->connection)
      {
	boost::shared_ptr<Query> q(new Query(query));
	if(!regExpr.empty())
	  q->setEORPattern(regExpr);
	this->connection->execute(q);
	return;
      }
      else
	BOOST_THROW_EXCEPTION( exception() << errorName(std::string("input_without_connection")) << errorTitle("Input was tried without connection assigned to device.") << errorSeverity(severity::ShouldNot) );
    }
    
    boost::shared_ptr<boost::asio::io_service> getIoService()
    {
      return this->ioService;
    }
    
    DeviceImpl() : connection(), ioService(new boost::asio::io_service), ioServiceWork(new boost::asio::io_service::work(*ioService)) , connected("connected")
    {
      this->threadPool.push_back(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, this->ioService))));
      this->threadPool.push_back(boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, this->ioService))));
      this->connected.onValueChange.connect(boost::bind(&DeviceImpl::emitConnectionSignals,this));
    }
    
    virtual ~DeviceImpl()
    {
      LUGHOS_LOG(log::SeverityLevel::informative) << "Device " << this->name << " is destructing." ;
      this->disconnect();
      ioService->stop();
      for(std::vector<boost::shared_ptr<boost::thread> >::iterator it = this->threadPool.begin(); it != this->threadPool.end(); it++)
      {
	(*it)->join();
      }
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