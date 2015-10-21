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
#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include <Wt/Dbo/QueryModel>
#include <boost/asio/io_service.hpp>
#include "measuredValue.hpp"
#include "jobQueue.hpp"

using namespace std;
using namespace lughos;
namespace dbo = Wt::Dbo;

template <class T>
class measuredDBValue : public measuredValue<T>
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

class fakeMeasurer : public Task
{
protected:
  
  boost::shared_ptr<dbo::Session> session;
  
  void run()
  {
    dbo::Transaction transaction(*session);
    this->session->add(static_cast<measuredDBValue*>(new measuredValue(this->executed,"bogus")));
    transaction.commit();
  }
  
public:
  fakeMeasurer(boost::shared_ptr<dbo::Session> session, boost::shared_ptr<boost::asio::io_service> ioService) : session(session), Task(ioService)
  {
    
  }
};

int main()
{
  dbo::backend::Sqlite3 sqlite3("test.db");
  boost::shared_ptr<dbo::Session> session(new dbo::Session);
  boost::shared_ptr<boost::asio::io_service> ioService(new boost::asio::io_service);
//   boost::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ioService));
//   boost::thread workerThread(boost::bind(&boost::asio::io_service::run, ioService));
    session->setConnection(sqlite3);
  session->mapClass<measuredDBValue>("measuredValue");
  try 
  {
    session->createTables();
    cout << "Creating tables..." << endl;
  }
  catch(...)
  {
    cout << "Tables already created." << endl;
  }
  fakeMeasurer faker(session,ioService);
  faker.setEvery(boost::posix_time::seconds(1));
  faker.setExecuteTimes(3);
  faker.start();
  ioService->run();
  cout << "Faker is done. See what we got in the DB..." << endl;
  dbo::Transaction transaction(*session);
  dbo::collection< dbo::ptr<measuredDBValue> > measuredValues = session->find<measuredDBValue>();
  std::cerr << "We have " << measuredValues.size() << " values in our database:" << std::endl;

  for (auto i = measuredValues.begin(); i != measuredValues.end(); ++i)
    std::cerr << " Value: " << (*i)->getvalue() << " " << (*i)->getunit() << " @ " << (*i)->getTimeStamp() << std::endl;
  
  typedef boost::tuple<double, boost::posix_time::ptime> Item;
  dbo::QueryModel<Item> *model = new dbo::QueryModel<Item>();
  model->setQuery(session->query<Item>("SELECT value, timestamp FROM measuredValue"));
  model->addColumn("value");
  model->addColumn("timestamp");
  cout << boost::any_cast<double>(model->data(1,0)) << " @ " << boost::any_cast<boost::posix_time::ptime>(model->data(1,1)) << endl;
  transaction.commit();
//   WTableView *view = new WTableView();
//   view->resize(600, 300);
//   view->setSelectionMode(SingleSelection);
//   view->setModel(model);
  return 0;
}