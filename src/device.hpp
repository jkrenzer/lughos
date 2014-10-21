#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>

#include "connectionImpl.hpp"

#define GUARD boost::lock_guard guard(mutex);

namespace lughos
{
  
  typedef boost::recursive_mutex Mutex;
  
  template <class C> DeviceTemplate 
  {
  protected:
    Mutex mutex;
    Connection<C> connection;
    bool initialized;
    
    virtual void initImplementation() = 0;
    
    virtual void shutdownImplementation() = 0;
    
    virtual std::string composeRequest(std::string query) = 0;
    
    virtual std::string interpretAnswer(std::string query) = 0;
    
    virtual std::string inputOutputImplementation(std::string query)
    {
      std::ostream requestStream(&connection.request);
      requestStream << this->composeRequest(query);
      connection.write();
      return this->interpretAnswer(connection.read());
    }
    
  public:
    
    void init()
    {
      GUARD
      this->initImplementation;
    }
    
    void shutdown()
    {
      GUARD
      this->shutdownImplementation;
    }
    
    std::string inputOutput(std::string query)
    {
      GUARD
      this->inputOutputImplementation(query);
    }
    
    DeviceTemplate<T>()
    {
      this->init();
    }
    
    ~DeviceTemplate<T>()
    {
      this->shutdown();
    }
    
  };
  
  template <class C> Device : public DeviceTemplate<C>
  {
    
  };

} //namespace lughos
#endif