#include <iostream>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WText>

#include "basicTypes.hpp"
#include "io.hpp"
#include "wtIo.hpp"
#include "transformations.hpp"

using namespace lughos;
using namespace Wt;

class testApplication : public WApplication
{
public:
  testApplication(const WEnvironment& env);

};

testApplication::testApplication(const WEnvironment& env) : WApplication(env)
{
  setTitle("Form Test");
  int zahl = 7;
  exposedPtr<int>* p = new exposedPtr<int>(&zahl,std::string("Zahl"),std::string("Irgendeine ,sZahl"));
  exposedValue<int> i(7);
  exposedValue<double> d(13.7);
  textLine<wtContext> box1(i);
  textLine<wtContext> box2(d);
  WWidget* w1 = box1.output(root());
  WWidget* w2 = box2.output(root());
  root()->addWidget(w1);
  root()->addWidget(w2);
}



WApplication *createApplication(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
   return new testApplication(env);
    
}

int main(int argc, char **argv) {
  try 
  {
    return WRun(argc, argv, &createApplication);
    //Test exposition of variable via pointer
    std::cout << "Testing form output" << std::endl;
    
   
    
  }
  catch(lughos::exception e)
  {
    std::cerr << lughos::makeErrorReport(e);
  }
    return 0;
}
