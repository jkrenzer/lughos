#ifndef CONNECTION_IMPL_HPP
#define CONNECTION_IMPL_HPP
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/detail/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/regex.hpp>

/**
 * @class ConnectionImpl
 * @brief class describing an abstract connection
 * 
 */
class ConnectionImpl
{
public:
        
  /**
   * @brief trys to establish connection
   * 
   * @return bool
   */
  virtual bool testconnection()=0;
  /**
   * @brief sets a port for the connection
   * 
   * @param port port name as string
   * @return void
   */
  virtual void set_port(std::string port)=0;
  /**
   * @brief sets hardware bits
   * 
   * @return void
   */
  virtual void reset()=0;
  /**
   * @brief aborts the connection
   * 
   * @return void
   */
  virtual void abort() = 0;
  /**
   * @brief sends query, waits for resonse
   * 
   * @param query string with pure command without end of line caracter
   * @return int returns 1 if succeeded
   */
  virtual int  write(std::string query, boost::regex regExpr = boost::regex())=0;
  /**
   * @brief sends query, does not wait for resonse
   * 
   * @param query string with pure command without end of line caracter
   * @return int returns 1 if succeeded
   */
  virtual int  write_only(std::string query)=0;
  /**
   * @brief waiter gives the device enough time to respond
   * 
   * @return void
   */
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
  /**
  * @brief notifies the waiting client when the query is done
  * 
  * @return void
  */
  void notifyWaitingClient()
	{
	  {
	    boost::lock_guard<boost::mutex> lock(waitForCompletionMutex);
	    this->queryDone=currentQuery.empty();
	  }
	  this->queryDoneCondition.notify_one();
	}
	/**
	 * @brief reads the response from the device
	 * 
	 * @return std::string uninterpreted response from device
	 */
	virtual std::string read()=0;
// 	connectionImpl(void);
// 	~connectionImpl(void);
protected:
  /**
   * @brief establishes the conntection
   * 
   * @return bool true if succeeded
   */
  virtual bool start()=0;
  /**
   * @brief stops the connection
   * 
   * @return void
   */
  virtual void stop()=0;
	boost::mutex waitForCompletionMutex;
	boost::condition_variable queryDoneCondition;
	std::string currentQuery;
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
