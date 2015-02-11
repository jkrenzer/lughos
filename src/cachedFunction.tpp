namespace lughos 
{  
  template <class R> R cachedFunction<R>::operator()()
  {
      if(!this->isValid() && readFunction)
      {
	  this->tainted = false;
	  return readFunction();
      }
      else
	  return cachedValue;
  }
  template <class R> void cachedFunction<R>::setReadFunction(boost::function< R() > readFunction)
  {
      this->readFunction = readFunction;
  }

  template <class R> void cachedFunction<R>::taint()
  {
      this->tainted = true;
  }

  template <class R> cachedFunction<R>::cachedFunction()
  {
      this->tainted = true;
      this->interval = boost::posix_time::seconds(1);
  }

  template <class R> bool cachedFunction<R>::isValid()
  {
      return (boost::posix_time::microsec_clock::local_time() - timestamp < interval ) && this->tainted;
  }
}//namespace lughos