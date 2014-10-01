#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <boost/regex.hpp>

#include "serialConnections.hpp"
// #include "Dict.hpp"
#include "serialAsync.hpp"


serialAsync::serialAsync(void) 
{
}

serialAsync::~serialAsync(void)
{

}



int serialAsync::write_async(const std::string &buf)
{    
  
	compose_request(buf);

	boost::system::error_code ec;
	const int size=buf.size();
// 	if (!port_) return -1;
	if (size == 0) return 0;
	 
   	 async_read_some_();
// 	  io_service_.run();
// 	       boost::asio::async_write(*port_, request_);
//     port_->async_write_some<>(request_);
	
	

	  
  return 1;
}


void serialAsync::async_read_some_()
{
  
//   std::cout<<"bist du versackt?"<<std::endl;
	if (port_.get() == NULL || !port_->is_open()) return;
// std::cout<<"nicht versackt"<<std::endl;
// 	port_->async_read_some( 
// 		boost::asio::buffer(read_buf_raw_, SERIAL_PORT_READ_BUF_SIZE),
// 		boost::bind(
// 			&serialAsync::handle_write_request,
// 			this, boost::asio::placeholders::error, 
// 			boost::asio::placeholders::bytes_transferred));
	
	      boost::asio::async_write(*port_, request_,
          boost::bind(&serialAsync::handle_write_request, this,
            boost::asio::placeholders::error));
	
	io_service_.run();
	
	
// 	std::string s = response_string_stream.str();
// 	response_string_stream.str("");
// 		  std::cout << s<< '\n';
}
// void serialAsync::on_receive_(const boost::system::error_code& ec, size_t bytes_transferred)
// {
// //   std::cout<<"on_receive_"<<std::endl;
// 	
// 	boost::mutex::scoped_lock look(mutex_);
// 
// 	if (port_.get() == NULL || !port_->is_open()) return;
// 	if (ec) {
// 		async_read_some_();
// 		return;
// 	}
// 
// 	for (unsigned int i = 0; i < bytes_transferred; ++i) {
// 		char c = read_buf_raw_[i];
// 		if (c == end_of_line_char_) {
// 			this->on_receive_(read_buf_str_);
// 			read_buf_str_.clear();
// 		}
// 		else {
// 			read_buf_str_ += c;
// 		}
// 	}
// 
// 	async_read_some_();
// }
// 
// void serialAsync::on_receive_(const std::string &data)
// {
// 	std::cout <<  data << std::endl;
// }


//----------------
//-------


void serialAsync::handle_write_request(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Read the response status line.
      boost::asio::async_read_until(*port_, response_, end_of_line_char_,
          boost::bind(&serialAsync::handle_read_content, this,
            boost::asio::placeholders::error));
    }
    else
    {
      std::cout << "Error: " << err.message() << "\n";
    }
  }
  
  

void serialAsync::handle_read_content(const boost::system::error_code& err)
  {

    if (!err)
    {
      // Write all of the data that has been read so far.
	response_string_stream<< &response_;
      // Continue reading remaining data until EOF.
	
// 	std::cout<<response_string_stream.str()<<std::endl;
//       boost::asio::async_read(*port_, response_,
//           boost::asio::transfer_at_least(1),
//           boost::bind(&serialAsync::handle_read_content, this,
//             boost::asio::placeholders::error));
    }
    else if (err != boost::asio::error::eof)
    {
      std::cout << "Error: " << err << "\n";
    }

  } 
//------------------


  void serialAsync::wait_callback(boost::asio::serial_port& port_, const boost::system::error_code& error)
  {
    //std::cout << " Calling wait-handler.";
    if (error)
    {
      // Data was read and this timeout was canceled
      return;
    }
    //std::cout << " Timed out.";
    port_.cancel();  // will cause read_callback to fire with an error
  } 

  