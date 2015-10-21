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
  if (values.size() > 0)
    cout << "Value " << values.front()->name << endl;
  transaction1.commit();
  
  cout << "We have " << values.size() << "values in the db." << std::endl;
  lughos::ExposedValue<int> zahl(12345,"Graf Zahl");
  lughos::PersistantRawValue* p = new PersistantRawValue(&zahl,&session);
  dbo::Transaction transaction(session);
  session.add<PersistantRawValue>(p);
  transaction.commit();
  
  return 0;
}