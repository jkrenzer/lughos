#include <iostream>
#include <ostream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using namespace std;

void doSomethingElse()
{
  for(int i = 0; i < 15; i++)
  {
    cout << "Boing #" << i << endl;
    boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }
}

void writeSomething(std::string s)
{
  std::cout << s << std::endl;
}

class Carl
{
public:
  virtual void do_something() = 0;
};

class Duffy : public Carl
{
public:
  boost::shared_ptr<boost::asio::io_service> ioService;
  
  Duffy(boost::shared_ptr<boost::asio::io_service> ioService) : ioService(ioService)
  {
  }
  
  void do_something()
  {
    boost::shared_ptr<boost::asio::serial_port> port(new boost::asio::serial_port(*ioService));
    boost::system::error_code ec;
    ioService->post(boost::bind(&writeSomething,std::string("Ok, i'll try too... *sigh*")));
    port->open("/dev/ttyUSB0", ec);
    std::string query("Hi!");
    boost::asio::async_write(*port,boost::asio::buffer(query),boost::bind(&writeSomething,std::string("And again: anybody hearing?!!!!")));
    port->close();
    ioService->post(boost::bind(&writeSomething,std::string("Content? And now be quiet already!!")));
    port.reset();
  }
};

int main()
{
  boost::shared_ptr<boost::asio::io_service> ioService(new boost::asio::io_service);
  boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ioService));
  ioService->post(boost::bind(&writeSomething,std::string("Ready, steady...")));
  boost::thread workerThread(boost::bind(&boost::asio::io_service::run, ioService));
  ioService->post(boost::bind(&writeSomething,std::string("GO!")));
  ioService->post(boost::bind(&writeSomething,std::string("Cewl, ioservice is still running! :-)")));
  ioService->post(boost::bind(&writeSomething,std::string("Now I'll open a serial port.")));
  boost::shared_ptr<boost::asio::serial_port> port(new boost::asio::serial_port(*ioService));
  boost::system::error_code ec;
  ioService->post(boost::bind(&writeSomething,std::string("Ok, object is here, now we'll give it a portname...")));
  port->open("/dev/ttyUSB0", ec);
  port->set_option(boost::asio::serial_port_base::baud_rate(9600));
  port->set_option(boost::asio::serial_port_base::character_size(8));
  port->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::hardware));
  port->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
  port->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
  std::string query("*IDN?\r\n");
  boost::asio::streambuf answerBuffer;
  istream answerStream(&answerBuffer);
  std::string answer;
  boost::asio::deadline_timer timer(*ioService);
  boost::asio::async_write(*port,boost::asio::buffer(query),boost::bind(&writeSomething,std::string("We have written something to the port. Anybody there?")));
  boost::asio::async_read_until(*port,answerBuffer,'\r',boost::bind(&writeSomething,std::string("We got an answer!")));
  answerStream >> answer;
//   timer.expires_from_now(boost::posix_time::milliseconds(1000));
//   timer.wait();
  ioService->run_one();
  ioService->post(boost::bind(&writeSomething,std::string("1: ") + answerStream.str()));
//   port->async_write_some(boost::asio::buffer(query),boost::bind(&writeSomething,std::string("Hello? Hello? Nobody answering?!")));
//   port->async_read_some(boost::asio::buffer(answerBuffer,64),boost::bind(&writeSomething,std::string("Ah! Hi! Nive to hear: ") + std::string(answerBuffer)));
//   boost::this_thread::sleep(boost::posix_time::millisec(5000));
  ioService->post(boost::bind(&writeSomething,std::string("2: ") + answerStream.str()));
  ioService->post(boost::bind(&writeSomething,std::string("Ha! I'm still alive!")));
  port->close();
  ioService->post(boost::bind(&writeSomething,std::string("Port closed...")));
  port.reset();
  ioService->post(boost::bind(&writeSomething,std::string("And port destroyed.")));
  boost::shared_ptr<Duffy> duffy( new Duffy(ioService));
  boost::shared_ptr<Carl> carl = duffy;
  ioService->post(boost::bind(&writeSomething,std::string("Lets ask carl if he can reach somebody...CAARL?! Im waiting....")));
  timer.expires_from_now(boost::posix_time::microseconds(10000000));
  ioService->post(boost::bind(&doSomethingElse));
  timer.wait();
  cout << "Hey, im still waiting!? Where is everybody?!" << endl;
  ioService->dispatch(boost::bind(&writeSomething,std::string("Now do it already!!")));
  carl->do_something();
  work.reset();
  workerThread.join();
  
  return 0;
}