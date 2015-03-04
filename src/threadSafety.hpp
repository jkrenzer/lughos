#ifndef THREAD_SAFETY_HPP
#define THREAD_SAFETY_HPP
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>

namespace lughos
{
   typedef boost::recursive_mutex Mutex;
   
   class ThreadSaveObject
   {
   protected:
     boost::upgrade_mutex mutex;
     
     typedef boost::shared_lock<boost::upgrade_mutex> SharedLock;
     
     typedef boost::upgrade_lock<boost::upgrade_mutex> UpgradeLock;
          
     typedef boost::unique_lock<boost::upgrade_mutex> ExclusiveLock;
     
     typedef boost::upgrade_to_unique_lock<boost::upgrade_mutex> upgradeLockToExclusive;
      
   };
   
}
#endif