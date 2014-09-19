#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

namespace lughos
{

template <class P> P transformation()
{
  P p;
  return p;
}

template <class T, class E> T transformation(E e) 
{
  return (T) e;
}

template <class T> std::string transformation(T &e)
{
  return std::string(e);
}

} //namespace lughos
#endif