#ifndef MEASURED_DB_VALUE_HPP
#define MEASURED_DB_VALUE_HPP

#include <iostream>
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include <boost/asio/io_service.hpp>
#include "measuredValue.hpp"
#include "jobQueue.hpp"

using namespace std;
using namespace lughos;
namespace dbo = Wt::Dbo;

class measuredDBValue : public measuredValue
{
public:
  
  template<class Action>
  void persist(Action& a)
  {
    dbo::field(a, timestamp,     "timestamp");
    dbo::field(a, sensorName, "sensorName");
    dbo::field(a, unit,     "unit");
    dbo::field(a, value,    "value");
  }
};

#endif
