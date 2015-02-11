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
    
    cachedFunction<R>();
    
    void taint();
    
    bool isValid();
    
    void setReadFunction(boost::function<R()> readFunction);
    
    R operator()();
   
  };
}//namespace lughos

#include "cachedFunction.tpp" //Necessary as templates cannot be generically instantiated from libraries!!
#endif
