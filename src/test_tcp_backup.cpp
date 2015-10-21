/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

#include <iostream>

// #include "tcpConnections.hpp"
// #include "connectionImpl.hpp"
#include "kithleigh.hpp"
#include <pthread.h>

int main(int argc, char **argv) {

//   http://1cckg5nsjmakrwwa.myfritz.net:6666/
  	bool rv;
	std::string server_name = "192.168.178.40";
// 	std::string port_name ="1cckg5nsjmakrwwa.myfritz.net:6666";
// 	std::string server_name ="www.boost.org";
	std::string name = server_name;


	Keithley* c = new Keithley();

	c->server_name=name; 
    	rv = c->start(); 
// 	std::cout << "rv="<< rv<< std::endl;
	if (rv == false) {
		return -1;
	}
 	  c->write("TRAC:CLE"); 
	  c->write("INT:CONT OFF");
// 	  c->write("TRIG:COUN 1");     
// 	  c->write("SAMP:COUN 10"); 
// 	  c->write("ROUT:SCAN:TSO (@101:110)"); 
//           c->write("ROUT:SCAN:TSO IMM");     
// 	  c->write("ROUT:SCAN:TSO INT"); 
	  c->write("READ?"); 
// 	    c->write("*IDN?");
	    c->write_async("*IDN?"); 
// 	    c->write("*IDN?"); 
//     	  std::cout << "Write="<< c->write_async("*IDN?")<< std::endl;
// 	  std::cout << "Write="<< c->write("*IDN?")<< std::endl;
// 	  std::cout << "Write="<< c->write("")<< std::endl;
// 	  std::cout << "Write="<< c->write_async("")<< std::endl;
// 	  	sleep(5);
       c->stop();
	return 0;
}
