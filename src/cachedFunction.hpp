#ifndef CACHED_FUNCTION_HPP
#define CACHED_FUNCTION_HPP
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>

namespace lughos
{

  template <class R> class cachedFunction
  {
  protected:
    boost::posix_time::ptime timestamp;
    boost::posix_time::time_duration interval;
    R cachedValue;
    boost::function<R()> readFunction;
    bool tainted;
    
  public:
    
    typedef R ReturnType;
    
    cachedFunction<R>()
    {
      this->tainted = true;
    }
    
    void taint()
    {
      this->tainted = true;
    }
    
    bool isValid()
    {
      return (boost::posix_time::microsec_clock::local_time() - timestamp < interval ) && this->tainted;
    }
    
    void setReadFunction(boost::function<R()> readFunction)
    {
      this->readFunction = readFunction;
    }
    
    R operator()()
    {
      if(!this->isValid() && readFunction)
      {
	this->tainted = false;
	return readFunction();
      }
      else
	return cachedValue;
    }
   
  };
}//namespace lughos
#endif