#ifndef MAKROS_HPP
#define MAKROS_HPP

//Automatically declare getter and setter-functions for variable X of type T

#define DECLARE_GETTER_SETTER(V,T) \
    void set ## V(T V);		\
				\
    const T get ## V() const;


//Automatically define getter and setter-functions for variable X of type T

#define DEFINE_GETTER_SETTER(C,V,T) \
    void C::set ## V(T V)		\
    {				\
      this->V = V;		\
    }				\
				\
    const T C::get ## V() const		\
    {				\
      return this->V;	\
    }

    
#endif