// #include "StdAfx.h"

// #include <Setupapi.h>
// #pragma comment(lib, "Setupapi.lib")

#include "kithleigh.hpp"


kithleigh::kithleigh(boost::asio::io_service* io_service): connection<serialContext>(io_service)
{
    this->dict = new httpDict;
    this->dict->addition = std::string("/scpi_response.html?cmd=");

}

kithleigh::~kithleigh(void)
{

}

   std::string kithleigh::inputoutput(const std::string input, const int async)
{
    if (async==0)write(input);
    else if (async==1)write_async(input);
    else write(input);
    return read();
}


