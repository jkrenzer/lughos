#include "test_gui.hpp"
#include "coolpak6000.hpp"

namespace lughos {

boost::asio::io_service * ioService;

} //namespace lughos

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new lughos::mainApplication(env);
}



int main(int argc, char **argv)
{
  lughos::ioService = new boost::asio::io_service;
  boost::asio::io_service::work work(*lughos::ioService);
  lughos::ioService->run();
  /*
   * Your main method may set up some shared resources, but should then
   * start the server application (FastCGI or httpd) that starts listening
   * for requests, and handles all of the application life cycles.
   *
   * The last argument to WRun specifies the function that will instantiate
   * new application objects. That function is executed when a new user surfs
   * to the Wt application, and after the library has negotiated browser
   * support. The function should return a newly instantiated application
   * object.
   */
  int result = Wt::WRun(argc, argv, &createApplication);
  lughos::ioService->stop();
  delete lughos::ioService;
  return result;
}

