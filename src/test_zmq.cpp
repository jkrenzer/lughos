#include <iostream>
#include <zmq.hpp>
#include <boost/program_options.hpp>
#include <vector>
#include <string>

typedef std::vector<std::string> Message;

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
  // Send reply back to client
  zmq::message_t reply (5);
  memcpy ((void *) reply.data (), "World", 5);
  socket.send (reply);
  }
}

void client(long int target_nbr)
{
  // Prepare our context and socket
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REQ);
  Message msg;
  
  std::cout << "Connecting to server…" << std::endl;
  socket.connect ("tcp://localhost:5555");

  // Do requests, waiting each time for a response
  zmq::message_t request (6);
  memcpy ((void *) request.data (), "Hello", 5);
  socket.send (request);

  // Get the reply.
  zmq::message_t reply;
  socket.recv (&reply);
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
  long int target_loops = 1000;
  boost::program_options::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("server,s" , "run as server")
      ("client,c" , "run as client")
      ("measurements,t", boost::program_options::value<long int>(&target_loops)->default_value(1000), "set how many measurements to take");
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
    client(target_loops);
    return 0;
  }
  else
  {
    std::cout << desc << std::endl;
    return 1;
  }
}