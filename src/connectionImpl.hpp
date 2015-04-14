#ifndef CONNECTION_IMPL_HPP
#define CONNECTION_IMPL_HPP
#include <boost/smart_ptr/shared_ptr.hpp>
#include "threadSafety.hpp"
#include "query.hpp"
#include <boost/chrono.hpp>
#include <boost/regex.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/signals2/signal.hpp>


namespace lughos
{

/**
* @class ConnectionImpl
* @brief class describing an abstract connection
*
*/
  class ConnectionImpl
  {
  public:

    ConnectionImpl ()
    {
    }

    virtual void ioService (boost::shared_ptr <
			    boost::asio::io_service > io_service)
    {
      this->io_service = io_service;
      this->timingStrand.reset(new boost::asio::strand(*io_service));
      this->ioStrand.reset(new boost::asio::strand(*io_service));
      this->timeoutTimer.reset (new boost::asio::deadline_timer (*io_service));
    } 
    
    boost::shared_ptr < boost::asio::io_service > ioService ()
    {
      return this->io_service;
    }

   /**
   * @brief trys to initialize connection
   *
   * @return bool
   */
    virtual bool test () = 0;

   /**
   * @brief checks if the connection was opened.
   *
   * @return bool
   */

    virtual bool connected () = 0;
   /**
    * @brief checks if the connection was initialized
    *
    * @return bool
    */

    virtual bool initialized () = 0;

   /**
   * @brief opens connection
   *
   * @return void
   */

    virtual void initialize () = 0;

   /**
   * @brief closes connection
   *
   * @return void
   */

    virtual void shutdown () = 0;

   /**
   * @brief sets hardware bits
   *
   * @return void
   */
    virtual void reset ()
    {
      this->abort ();
      this->shutdown ();
      this->initialize ();
    }
   /**
   * @brief aborts all actions on connection
   *
   * @return void
   */
    virtual void abort () = 0;
   /**
   * @brief puts query in queue and processes received answers.
   *
   * @param query object of Query-type
   * @return void
   */
    virtual void execute (boost::shared_ptr < Query > query) = 0;
   /**
   * @brief sends query, does not wait for resonse
   *
   * @param query string with pure command without end of line caracter
   * @return int returns 1 if succeeded
   */

  protected:

    boost::mutex busy;
    boost::shared_ptr < boost::asio::io_service > io_service;
    boost::shared_ptr < boost::asio::deadline_timer > timeoutTimer;
    boost::shared_ptr < boost::asio::strand> timingStrand;
    boost::shared_ptr < boost::asio::strand> ioStrand;

    //    virtual int write(const std::string &buf)=0;
    //    virtual int write_async(const std::string &buf)=0;




  };

template < class C > class ConnectionTemplate:public ConnectionImpl
  {
  public:
    C context;
  };

template < class C > class Connection:public ConnectionTemplate < C >
  {
  };

}				// namespace lughos
#endif
// kate: indent-mode cstyle; indent-width 2; replace-tabs on; ;
