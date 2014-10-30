#include <iostream>
#include <boost/smart_ptr/shared_ptr.hpp>
using namespace std;

class Var
{
public:
  int value;
  
  Var()
  {
    value = -100;
  }
  
  void plus100()
  {
    value += 100;
  }
};

class A
{
public:
  int A;
  
  virtual std::string foo() = 0;
};

class B : public A
{
public:
  std::string B;
  Var v;
  
  std::string foo()
  {
    v.plus100();
    return B + std::string(to_string(v.value));
  }
  
};

int main()
{
  boost::shared_ptr<B> b(new B);
  b->B = std::string("Ich foo hier herum!");
  cout << b->foo() << endl;
  b->B = std::string("Ich foo nochmal!");
  boost::shared_ptr<A> a;
  a= b;
  cout << a->foo() << endl;
  a = boost::shared_ptr<A>(b);
  cout << a->foo() << endl;
  return 0;
}