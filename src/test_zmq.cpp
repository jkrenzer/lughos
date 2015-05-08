#include <iostream>
#include <zmq.hpp>
#include <boost/program_options.hpp>
using namespace std;

void server()
{
  // Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  socket.bind ("tcp://*:5555");

  while (true) {
  zmq::message_t request;

  // Wait for next request from client
  socket.recv (&request);
  std::cout << "Received Hello" << std::endl;

  // Do some 'work'
  sleep(1);

  // Send reply back to client
  zmq::message_t reply (5);
  memcpy ((void *) reply.data (), "World", 5);
  socket.send (reply);
  }
}

void client()
{
  // Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REQ);

  std::cout << "Connecting to hello world server…" << std::endl;
  socket.connect ("tcp://localhost:5555");

  // Do 10 requests, waiting each time for a response
  for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
  zmq::message_t request (6);
  memcpy ((void *) request.data (), "Hello", 5);
  std::cout << "Sending Hello " << request_nbr << "…" << std::endl;
  socket.send (request);

  // Get the reply.
  zmq::message_t reply;
  socket.recv (&reply);
  std::cout << "Received World " << request_nbr << std::endl;
  }
}

/* Function used to check that 'opt1' and 'opt2' are not specified
   at the same time. */
void conflicting_options(const boost::program_options::variables_map& vm, 
                         const char* opt1, const char* opt2)
{
    if (vm.count(opt1) && !vm[opt1].defaulted() 
        && vm.count(opt2) && !vm[opt2].defaulted())
        throw logic_error(string("Conflicting options '") 
                          + opt1 + "' and '" + opt2 + "'.");
}

int main(int argc, char **argv)
{
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("server,s" , "run as server")
      ("client,c" , "run as client");
  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  boost::program_options::notify(vm);
  try 
  {
    conflicting_options(vm, "server", "client");
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  
  if (vm.count("help")) 
  {
    std::cout << desc << std::endl;
    return 1;
  }
  if (vm.count("server"))
  {
    server();
    return 0;
  }
  else if (vm.count("client"))
  {
    client();
    return 0;
  }
  else
  {
    std::cout << desc << std::endl;
    return 1;
  }
}