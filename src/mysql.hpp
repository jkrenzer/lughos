#ifndef MYSQL_HPP
#define MYSQL_HPP
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
  }//namespace lughos
  #endif