#ifndef LUGHOS_OBJECTS_HPP
#define LUGHOS_OBJECTS_HPP
#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;

namespace lughos
{

class configurableObject
{
public:
  ptree config;  
};
  
}//namespace lughos
#endif