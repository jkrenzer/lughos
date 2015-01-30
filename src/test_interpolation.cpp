#include <iostream>
#include "interpolation.hpp"

using namespace std;
using namespace lughos;

int main()
{
  SplineTransformation t;
  SplineTransformation::XToYMap& x2y = t.valueMap.left;

  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0000", NULL, 0), 0.645));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0100", NULL, 0), 3.270));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0200", NULL, 0), 6.373));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0300", NULL, 0), 9.489));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0400", NULL, 0),12.592));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0500", NULL, 0),15.702));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0600", NULL, 0),18.817));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0700", NULL, 0),21.919));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0800", NULL, 0),25.028));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0900", NULL, 0),28.137));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0A00", NULL, 0),31.247));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0B00", NULL, 0),34.355));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0C00", NULL, 0),37.471));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0D00", NULL, 0),39.489));
  x2y.insert(SplineTransformation::XYPair((double)strtol("0x0E00", NULL, 0),39.490));
  t.init();
  long int a = 0;
  for (int i = 0; i < 100; i++)
  {
    a = i*strtol("0x0010", NULL, 0);
    cout << "x=" << a << " y=" << t.xToY(a) << std::endl;
     
  }
  cout << "--> x=" << strtol("0x0F00", NULL, 0) << " y=" << t.xToY(strtol("0x0E00", NULL, 0)) << std::endl;
  return 0;
}