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
  
template <class P, class E> class transform
{
public:
  P to(E e)
  {
    return (P) e;
  }
  
  P to(P p)
  {
    return (E) p;
  }
  
};

template <class E> class transform<E,E>
{
public:
  E to(E e)
  {
    return e;
  }
};


template <class E> class transform<std::string,E>
{
public:
  std::string to(E e)
  {
    std::stringstream ss;
    ss << e;
    return ss.str();
  }
  
  E to(std::string str)
  {
    std::stringstream ss(str);
    E e;
    ss >> e;
    return e;
  }
};

} //namespace lughos
#endif