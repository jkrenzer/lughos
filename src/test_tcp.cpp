#include <iostream>

#include "tcpConnections.hpp"
#include "connectionImpl.hpp"
#include <pthread.h>

int main(int argc, char **argv) {

//   http://1cckg5nsjmakrwwa.myfritz.net:6666/
  	bool rv;
// 	std::string port_name = "192.168.178.40";
	std::string port_name ="www.boost.org";
	std::string name = port_name;

  
    connection<tcpContext>* c = new connection<tcpContext>();

 
    	rv = c->start(name.c_str()); 

	    	  std::cout << "rv="<< rv<< std::endl;
	if (rv == false) {
		return -1;
	}
                

//     	  std::cout << "Write="<< c->write_async("*IDN?")<< std::endl;
// 	  std::cout << "Write="<< c->write("*IDN?")<< std::endl;
	  std::cout << "Write="<< c->write("")<< std::endl;
	  std::cout << "Write="<< c->write_async("")<< std::endl;
// 	  	sleep(5);

		
	return 0;
}
