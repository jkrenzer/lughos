#ifndef UTILS_HPP
#define UTILS_HPP
#include <boost/lexical_cast.hpp>

namespace lughos
{

  template <class T, class S> T save_lexical_cast(S& source, T saveDefault)
  {
    try
    {
      return boost::lexical_cast<T>(source);
    }
    catch(boost::bad_lexical_cast e)
    {
      return saveDefault;
    }
    
  }
  
}
#endif