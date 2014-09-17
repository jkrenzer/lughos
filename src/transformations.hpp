#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP

namespace lughos
{

template <class P> class transformationTemplate
{
public:
 
  virtual const P& operator()() = 0;
     
};

template <class P> class transformation : public transformationTemplate<P>
{
};

} //namespace lughos
#endif