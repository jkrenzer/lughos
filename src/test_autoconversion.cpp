#include <iostream>

using namespace std;

template <class T> T& toType(boost::shared_ptr<d> &v)
{
  return boost::shared_ptr<>static_cast<boost::shared_ptr<T>>(v);
}

int main()
{
  double z;
  z = 123.456;
  boost::shared_ptr<d> v = (boost::shared_ptr<d>)&z;
  cout << v << " : " << toType<double>(v);
  
  return 0;
}