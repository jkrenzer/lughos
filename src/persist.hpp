#ifndef PERSIST_HPP
#define PERSIST_HPP

#include "exposedValues.hpp"

#include <Wt/Dbo/Dbo>


// Include the Connector/C++ headers
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
 
// Link to the Connector/C++ library
#pragma comment(lib, "mysqlcppconn.lib")

namespace lughos {

class MysqlConnector
{
protected:
  sql::Driver     *driver; // Create a pointer to a MySQL driver object
  sql::Connection *dbConn; // Create a pointer to a database connection object
  sql::Statement  *stmt;   // Create a pointer to a Statement object to hold our SQL commands
  sql::ResultSet  *res;    // Create a pointer to a ResultSet object to hold the results of any queries we run
  
  MysqlConnector(std::string url, std::string username, std::string password)
  {
  try
    {
	    driver = get_driver_instance();
    }
    catch (sql::SQLException e)
    {
	    std::cout << "Could not get a database driver. Error message: " << e.what() << std::endl;

    }

    // Try to connect to the DBMS server
    try
    {
	    dbConn = driver->connect(url, username, password);
    }
    catch (sql::SQLException e)
    {
	    std::cout << "Could not connect to database. Error message: " << e.what() << std::endl;
    }
  }
  
  void execute(std::string command)
  {
    try
    {
      stmt = dbConn->createStatement(); // Specify which connection our SQL statement should be executed on
      stmt->execute(command);
    }
    catch (sql::SQLException e)
    {
      std::cout << "Error while executing command. Error message: " << e.what() << std::endl;
    }
    delete stmt;
  }
  
  void query(std::string command)
  {
    try
    {
      stmt = dbConn->createStatement(); // Specify which connection our SQL statement should be executed on
      res = stmt->executeQuery(command);
    }
    catch (sql::SQLException e)
    {
      std::cout << "Error while executing query. Error message: " << e.what() << std::endl;
    }
    delete stmt;
  }
  
  ~MysqlConnector()
  {
    delete res;
    delete stmt;
    delete dbConn;
  }
  
};
  
class persistContext
{
};

class wtContext
{
};

namespace dbo = Wt::Dbo;

class PersistantRawValue
{
protected:
  TreeNode* linkedObj;
  dbo::collection<dbo::ptr<PersistantRawValue> > children;
  dbo::ptr<PersistantRawValue> parent;
  
  dbo::Session* session;
public:
  
  std::string name;
  
  PersistantRawValue()
  {
    this->session = NULL;
    this->linkedObj = NULL;
  }
  
  PersistantRawValue(dbo::Session* session)
  {
    this->linkedObj = NULL;
    this->session = session;
  }
  
  PersistantRawValue(TreeNode* eO,dbo::Session* session)
  {
    this->sync(eO);
    this->linkedObj = eO;
    this->session = session;
  }
  
  void sync(TreeNode* eO)
  {
    this->linkedObj = eO;
    if(this->session != NULL && this->linkedObj != NULL)
    {
      children.clear();
      this->name = eO->getName();
      std::vector<boost::shared_ptr<TreeNode>> eOChildren;
      for(std::vector<boost::shared_ptr<TreeNode>>::iterator it = eOChildren.begin(); it < eOChildren.end();it++)
      {
	this->children.insert(dbo::ptr<PersistantRawValue>(this->session->add(new PersistantRawValue(it->get(),this->session))));
      }
      this->parent = dbo::ptr<PersistantRawValue>(this->session->add(new PersistantRawValue(eO->getParent<TreeNode>().get(),this->session)));
  }
  }
  
  void sync()
  {
    this->sync(linkedObj);
  }
  
  template<class Action>
  void persist(Action& a)
  {
    dbo::field(a,name,"name");
    dbo::hasMany(a,children,dbo::ManyToOne,"children");
    dbo::belongsTo(a,parent,"children");
  }
  
};



template <class C> class Persistor
{
protected:
  
public:
  
};

template <> class Persistor<wtContext>
{
protected:
  
public:
  
  
  
};

}

#endif