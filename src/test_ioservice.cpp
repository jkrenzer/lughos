#include <iostream>
#include <ostream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "device.hpp"
#include "jobQueue.hpp"

using namespace std;

using namespace lughos;

class TestTask : public Task
{
public:
  
  std::string name;
  
    TestTask(boost::shared_ptr< boost::asio::io_service > executionQueuePtr) : Task(executionQueuePtr)
    {
      
    }
  
  void run()
  {
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration td = now - this->lastExecution - this->every;
    cout << "It's " << name << " I've been executed " << this->executed << " times. Time-inprecision: " << td <<  endl;
  }
  
};

int main()
{
  boost::shared_ptr<boost::asio::io_service> ioService(new boost::asio::io_service);
  boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ioService));
  boost::thread workerThread(boost::bind(&boost::asio::io_service::run, ioService));
  TestTask test(ioService);
  test.setEvery(boost::posix_time::seconds(1));
  test.setExecuteTimes(12);
  test.name = std::string("Albert");
  test.start();
  TestTask test2(ioService);
  test2.setEvery(boost::posix_time::seconds(3));
  test2.setExecuteTimes(Task::Execute::infinite);
  test2.name = std::string("Bert");
  test2.start();
  cout << "This is main-thread. Waiting for work to end..." << endl;
  work.reset();
  workerThread.join();
  
  return 0;
}
