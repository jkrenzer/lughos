#ifndef DICT_HPP
#define DICT_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 

class Dict
{
  public:                              // öffentlich
    Dict();                      // der Default-Konstruktor
    ~Dict();                     // der Destruktor

 public: 
   virtual void check_response(boost::asio::streambuf* response)=0;  
   virtual void compose_request(std::string server, const std::string &buf, boost::asio::streambuf* request)=0; 
// protected:
    std::string addition;

};
#endif