#include<iostream>
#include "persist.hpp"
#include <Wt/Dbo/backend/Sqlite3>
using namespace std;

using namespace lughos;

int main()
{
  dbo::backend::Sqlite3 sqlite3("blog.db");
  dbo::Session session;
  session.setConnection(sqlite3);
  session.mapClass<PersistantRawValue>("PersistantRawValue");
//   session.createTables();
  typedef dbo::collection< dbo::ptr<lughos::PersistantRawValue> > Values;
  dbo::Transaction transaction1(session);
  Values values = session.find<PersistantRawValue>();
  cout << "Value " << values.front().get()->name << endl;
  transaction1.commit();
  
  cout << "We have " << values.size() << "values in the db." << std::endl;
  lughos::ExposedValue<int> zahl(12345,"Graf Zahl");
  lughos::PersistantRawValue* p = new PersistantRawValue(&zahl,&session);
  dbo::Transaction transaction(session);
  session.add<PersistantRawValue>(p);
  transaction.commit();
  
  return 0;
}