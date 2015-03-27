#ifndef THREAD_SAFETY_HPP
#define THREAD_SAFETY_HPP
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

namespace lughos
{
   typedef boost::upgrade_mutex Mutex;
  
   typedef boost::shared_lock<Mutex> SharedLock;
     
   typedef boost::upgrade_lock<Mutex> UpgradeLock;
          
   typedef boost::unique_lock<Mutex> ExclusiveLock;
     
   typedef boost::upgrade_to_unique_lock<Mutex> upgradeLockToExclusive;
   
   
   
}
#endif