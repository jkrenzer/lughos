#ifndef THREAD_SAFETY_HPP
#define THREAD_SAFETY_HPP
#define GUARD boost::lock_guard<boost::recursive_mutex> guard(mutex);
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/locks.hpp>

namespace lughos
{
   typedef boost::recursive_mutex Mutex;
}
#endif