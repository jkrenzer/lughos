#include <iostream>
#include <fstream>

#include "tcpConnections.hpp"
#include "Dict.hpp"

#ifdef WIN32
#include <windows.h>
#include <winioctl.h>
#endif

Connection<tcpContext>::Connection() : request(), response(), endpoint(new tcp::endpoint())
{
this->connected = false;
}

Connection<tcpContext>::~Connection(void)
{
// 	stop();
}

bool Connection<tcpContext>::start()
{
    if (server_name.empty()||port_name.empty()) {
		std::cout << "please set server/port name before start" << std::endl;
		return false;
	}
    std::cout << "server: "<<server_name<<" port: "<<port_name << std::endl;
    resolver= boost::shared_ptr<tcp::resolver>(new tcp::resolver(*this->io_service_));
    query= boost::shared_ptr<tcp::resolver::query>(new tcp::resolver::query(server_name, port_name));
    socket= boost::shared_ptr<boost::asio::ip::tcp::socket>(new boost::asio::ip::tcp::socket(*this->io_service_));
    boost::asio::socket_base::keep_alive keepAlive(true);
//     socket->set_option(keepAlive); //Seems to be only allowed after connect :/
    this->connected = false;
    return true;
}



void Connection<tcpContext>::reset()
{
}


void Connection<tcpContext>::stop()
{

    try
    {
	resolver->cancel();
      if (query) 
	{  

	}
    }
    catch(...)
    {
      std::cout<<"stop failed"<<std::endl;
    }

}



void Connection<tcpContext>::compose_request(const std::string &buf)
{

}


void Connection<tcpContext>::set_port(std::string port)
{
  port_name=port;
}


// std::string Connection<tcpContext>::read()
// {
//         std::string s = response_string_stream.str();
// 	response_string_stream.str(std::string(""));
// // 	stop();
//     return s;  
// 
// }

bool Connection<tcpContext>::testconnection()
{
  start();
  bool ConnectionEstablished = false;
     try 
     {
      ConnectionEstablished = this->start();
     }
     catch(...)
     {
       ConnectionEstablished = false;
     }
     if(ConnectionEstablished)
     {
       this->stop();
     }
     return ConnectionEstablished;
}

int Connection<tcpContext>::write(std::string query)
{
  return 0;  
}

int Connection<tcpContext>::write_only(std::string query)
{
  return 0;  
}


void Connection<tcpContext>::set_default()
{
    
}