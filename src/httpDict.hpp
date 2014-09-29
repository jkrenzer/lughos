#ifndef HTTP_DICT_HPP
#define HTTP_DICT_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp> 
#include "Dict.hpp"

class httpDict : public Dict
{
  public:                              // öffentlich
    httpDict();                      // der Default-Konstruktor
    ~httpDict();                     // der Destruktor

 public: 
    void check_response(boost::asio::streambuf* response);  
    void compose_request(std::string server, const std::string &buf, boost::asio::streambuf* request); 
// protected:
//     std::string addition;
protected:
    std::string host_path;

};
#endif