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
  Pointer<int>* p = new Pointer<int>(&zahl,std::string("Zahl"));
  Value<int> i(7);
  Value<double> d(13.7);
  i.setName("Inge Integer");
  d.setName("Dorothea Double");
  p->setName("Peter Pointer");
  textLine<wtContext> box1(*p);
  textLine<wtContext> box2(d);
  WWidget* w1 = box1.output();
  WWidget* w2 = box2.output();
  WWidget* w3 = new WText("Test of formdisplay");
  WWidget* w4 = new WText("yay!");
  WTemplate* t1 = new WTemplate("<div style=\"color:red;\">${text1}</div><div style=\"color:green;\">${text2}</div>");
  t1->bindWidget("text1",w3);
  t1->bindWidget("text2",w4);
  WWidget* w5 = t1;
//   root()->addWidget(t1);
  root()->addWidget(w5);
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
