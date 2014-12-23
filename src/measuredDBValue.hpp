#ifndef MEASURED_DB_VALUE_HPP
#define MEASURED_DB_VALUE_HPP

#include <iostream>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include <boost/asio/io_service.hpp>
#include "measuredValue.hpp"
#include "jobQueue.hpp"
#include "device.hpp"



using namespace std;
namespace lughos
{

class Channel;
class measuredDBValue : public measuredValue
{
public:
  
  Wt::Dbo::ptr<Channel> channel;
//   Wt::Dbo::collection< Wt::Dbo::ptr<Channel> > channel;
  template<class Action>
  
  void persist(Action& a)
  {
    Wt::Dbo::field(a, timestamp,     "timestamp");
    Wt::Dbo::field(a, sensorName, "sensorName");
    Wt::Dbo::field(a, unit,     "unit");
    Wt::Dbo::field(a, value,    "value");
//     Wt::Dbo::belongsTo(a, channel, "measuredDBValue");
  }

};
}
#endif
