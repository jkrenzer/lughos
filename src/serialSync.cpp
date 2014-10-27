// #include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "serialConnections.hpp"
// #include "Dict.hpp"
#include "serialSync.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time.hpp>
#include <boost/iterator.hpp>
#include <string>
//LATERON ADD SUPPORT FOR CROSSCOMPILING!
#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
# define BOOST_ASIO_OPTION_STORAGE DCB
#else
# define BOOST_ASIO_OPTION_STORAGE termios
#include <sys/ioctl.h>
#endif

// tcpSync::tcpSync(void)
// {
// 
// }
serialSync::serialSync(boost::asio::io_service* io_service) : Connection< serialContext >(io_service)
{
}

serialSync::~serialSync(void)
{
}



int serialSync::write(std::string query)
{
  start();
//   io_service_->run();
  boost::system::error_code error = boost::asio::error::host_not_found;
  std::ostream request_stream(&request);

    request_stream<<query;
  
  boost::asio::write(*port_, request);

// Read the response status line.
  if(end_of_line_char_=='\0'){
    boost::asio::read(*port_, response,
          boost::asio::transfer_at_least(4), error);
//     boost::asio::read_until(*port_, response_, end_of_line_char_);  
  }
  else  boost::asio::read_until(*port_, response, end_of_line_char_);
//     handle_read_check_response(error);
//      boost::asio::read_until(*port_, response_, "K");
      handle_read_check_response(error);

    response_string_stream<< &response;

    return 1;
}



