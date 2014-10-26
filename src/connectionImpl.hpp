#ifndef CONNECTION_IMPL_HPP
#define CONNECTION_IMPL_HPP
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/detail/thread.hpp>
#include <boost/chrono.hpp>

class ConnectionImpl
{
public:
        
	virtual bool testconnection()=0;
	virtual void set_port()=0;
	virtual void reset()=0;
	virtual void abort() = 0;
	virtual int  write(std::string query)=0;
	void waitForCompletion()
	{
	  boost::unique_lock<boost::mutex> lock(waitForCompletionMutex);
	  queryDoneCondition.wait_for(lock,boost::chrono::milliseconds(2000));
	  std::cout << "Waking.." << std::endl;
	  
	  if(queryDone)
	  {
	    std::cout << "All done." << std::endl;
	  }
	  else
	  {
	    std::cout << "Timeout." << std::endl;
	    this->abort();
	  }
	}
	void notifyWaitingClient()
	{
	  {
	    boost::lock_guard<boost::mutex> lock(waitForCompletionMutex);
	    this->queryDone=true;
	  }
	  this->queryDoneCondition.notify_one();
	}
	virtual std::string read()=0;
// 	connectionImpl(void);
// 	~connectionImpl(void);
protected:
	virtual bool start()=0;
	virtual void stop()=0;
	boost::mutex waitForCompletionMutex;
	boost::condition_variable queryDoneCondition;
	bool queryDone;
// 	virtual int write(const std::string &buf)=0;
// 	virtual int write_async(const std::string &buf)=0;


	

};

template <class C> class ConnectionTemplate : public ConnectionImpl
{
public:
  C context;
};

template <class C> class Connection : public ConnectionTemplate<C>
{};
#endif