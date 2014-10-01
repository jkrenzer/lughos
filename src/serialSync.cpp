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
serialSync::serialSync(void)
{
}

serialSync::~serialSync(void)
{
}



int serialSync::write(const std::string &buf)
{

	io_service_.run();


	this->compose_request(buf);

//                std::cout<<"vor write:"<<&this->request_<<std::endl;
//      
     boost::system::error_code error = boost::asio::error::host_not_found;
     
//      boost::asio::streambuf stream;
     boost::asio::write(*port_, request_);
//                     std::cout<<"nach write:"<<&this->request_<<std::endl;
     
//           boost::asio::streambuf stream;
//       const std::string  buf1="*IDN?\r";
      
//           std::ostream request_stream(&stream);

//     request_stream <<buf.c_str()<< "\r";
//       std::cout<<"composed_"<<&request_<<std::endl;
//        boost::asio::write(*port_,boost::asio::buffer(buf1.c_str(),buf1.size()));
//          boost::asio::write(*port_,stream);
     
//   if(&request_)    std::cout<<"composed_"<<std::endl;
// 
  
  
//           char c;
//         std::string result;
//         for(;;)
//         {
//             boost::asio::read(*port_,boost::asio::buffer(&c,1));
//             switch(c)
//             {
//                 case '\r':
//                     break;
//                 case '\n':
//                      std::cout<<result<<std::endl;;
//                 default:
//                     result+=c;
//             }
//         }
//     // Read the response status line.
    boost::asio::read_until(*port_, response_, end_of_line_char_);
//     handle_read_check_response(error);
//      boost::asio::read_until(*port_, response_, "K");
      handle_read_check_response(error);
// //     // Read the response headers, which are terminated by a blank line.
//     boost::asio::read_until(port_, response_, end_of_line_char_);
// // 
// //     serialSync::handle_read_headers_process();
// 
//     // Read until EOF, writing data to output as we go.
//         while (boost::asio::read(*port_, response_, boost::asio::transfer_at_least(1), error))
    response_string_stream<< &response_;
	
// 	 std::cout<<"response: "<<&this->response_<<std::endl;


    
//     if (error != boost::asio::error::eof)
//       throw boost::system::system_error(error);
  

	
// 	 static const boost::regex e("<body>(.*)</body>");
// 	 boost::cmatch res;
// 	 boost::regex_search(s.c_str(), res, e);
// 	  std::cout << res[1] << '\n';
    
    return 1;
}



