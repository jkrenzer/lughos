#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP
#include <string>
#include <sstream>

namespace lughos
{

// template <class P> P transformation()
// {
//   P p;
//   return p;
// }
// 
// template <class T, class E> T transformation(E e) 
// {
//   return (T) e;
// }
// 
// template <class T> T transformation(T e)
// {
//   return e;
// }
// 
// template <class T> std::string transformation(T &e)
// {
//   return std::string(e);
// }
  
template <class P> class transformTo
{
public:
  template <class E> static P from(E e)
  {
    return (P) e;
  }
};

template <> class transformTo<std::string>
{
public:
  template <class E> static std::string from(E e)
  {
    std::stringstream ss;
    ss << e;
    return ss.str();
  }
};

template <> template <> int transformTo<int>::from<std::string>(std::string e)
{
  return std::stoi(e);
}

template <> template <> double transformTo<double>::from<std::string>(std::string e)
{
  return std::stod(e);
}

} //namespace lughos
#endif