#include <iostream>
#include "basicTypes.hpp"
#include "io.hpp"
#include "transformations.hpp"

using namespace lughos;

int main(int argc, char **argv) {
  try 
  {
    //Test exposition of variable via pointer
    std::cout << "Testing form output" << std::endl;
    int zahl = 7;
    exposedPtr<int>* test = new exposedPtr<int>(&zahl,std::string("Zahl"),std::string("Irgendeine Zahl"));
    transformation< std::string > t; 
    std::cout << t(*test);
    
    renderValue<consoleContext,int> r(*test);
    r.render();
    
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
    return 0;
}
