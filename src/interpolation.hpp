#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/bimap/bimap.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>

#include "threadSafety.hpp"

namespace lughos
{
  class SplineTransformation
  {
  private:
  mutable Mutex mutex;
  public:
    typedef boost::bimaps::bimap< boost::bimaps::set_of<double>, boost::bimaps::set_of<double> > ValueMap;
    typedef ValueMap::left_map XToYMap;
    typedef ValueMap::right_map YToXMap;
    typedef ValueMap::left_map::value_type XYPair;
    typedef ValueMap::left_map::value_type YXPair;
    

  protected:
    bool isInitialized;
    double xMax;
    double xMin;
    double yMax;
    double yMin;
    double xMaxLimit;
    double xMinLimit;
    double yMaxLimit;
    double yMinLimit;
    gsl_interp_accel *xToYSplineAcc;
    gsl_spline *xToYSpline;
    gsl_interp_accel *yToXSplineAcc;
    gsl_spline *yToXSpline;
    
  public:
    ValueMap valueMap;
    
    SplineTransformation()
    {
      ExclusiveLock lock(mutex);
      this->isInitialized = false;
      this->xToYSplineAcc = nullptr;
      this->xToYSpline = nullptr;
      this->yToXSplineAcc = nullptr;
      this->yToXSpline = nullptr;
      this->xMaxLimit = std::numeric_limits<double>::infinity();
      this->yMaxLimit = std::numeric_limits<double>::infinity();
      this->xMinLimit = -std::numeric_limits<double>::infinity();
      this->yMinLimit = -std::numeric_limits<double>::infinity();
    }
    
    ~SplineTransformation()
    {
      if(isInitialized)
      {
	gsl_spline_free (this->xToYSpline);
        gsl_interp_accel_free (this->xToYSplineAcc);
	gsl_spline_free (this->yToXSpline);
        gsl_interp_accel_free (this->yToXSplineAcc);
      }
    }
    
    bool fromFile(std::string filePath)
    {
      ExclusiveLock lock(mutex);
      XToYMap& x2y = this->valueMap.left;
      int validPairsRead = 0;

      try 
      {
	std::ifstream infile(filePath);
	boost::regex regEx("^(\\d*\\.?\\d*([eE][+-]\\d\\d)?)[\\D,]+(\\d*\\.?\\d*([eE][+-]\\d\\d)?)$");
	boost::match_results<std::string::iterator> result;
	std::string line;
	std::stringstream ss;
	std::cout << "Reading from file " << filePath << std::endl;
	while(std::getline(infile, line))
	{
	  boost::regex_search(line.begin(),line.end(),result,regEx);
	  if(result.size() > 2)
	  {
	    try
	    {
// 	      std::cout << "Got results: " << result[0] << " - " << result[1] << " - " << result[3] << std::endl;
	      x2y.insert(XYPair(boost::lexical_cast<double>(result[1]),boost::lexical_cast<double>(result[3])));
// 	      std::cout << "Read line with values: " << boost::lexical_cast<double>(result[1]) << ", " << boost::lexical_cast<double>(result[3]) << std::endl;
	      validPairsRead++;
	    }
	    catch(...)
	    {
	      std::cout << "Line ignored." << std::endl;
	    }
	  }
	  else
	    std::cout << "Line ignored..";
	}
      }
      catch(...)
      {
	return false;
      }
      std::cout << "Read " << validPairsRead << " value-pairs from file " << filePath << std::endl;
      return init();
    }
    
    void toFile(std::string filePath)
    {
      //TODO
    }
    
    bool init()
    {
      ExclusiveLock lock(mutex);
      gsl_set_error_handler_off();
      XToYMap& x2y = this->valueMap.left;
      int count = x2y.size();
      if (count < 2)
	return false;
      
      int i = 0;
      double x[count];
      double y[count];
      for(XToYMap::const_iterator it = x2y.begin(); it != x2y.end(); it++)
      {
	x[i] = it->first;
	y[i] = it->second;
	std::cout << "x=" << it->first << " y=" << it->second << std::endl;
	i++;
      }
      this->xMin = x[0];
      this->yMin = y[0];
      this->xMax = x[count-1];
      this->yMax = y[count-1];
      std::cout << "xMin: " << this->xMin << " yMin:" << this->yMin << std::endl;
      std::cout << "xMax: " << this->xMax << " yMax:" << this->yMax << std::endl;
      try 
      {
	this->xToYSplineAcc = gsl_interp_accel_alloc ();
	this->xToYSpline = gsl_spline_alloc (gsl_interp_linear, count);
	this->yToXSplineAcc = gsl_interp_accel_alloc ();
	this->yToXSpline = gsl_spline_alloc (gsl_interp_linear, count);
	gsl_spline_init (this->xToYSpline, x, y, count);
	gsl_spline_init (this->yToXSpline, y, x, count);
	this->isInitialized = true;
      }
      catch(...)
      {
	return false;
      }
      return true;
    }
    
    double extrapolate(double value, double refValue,gsl_spline* spline, gsl_interp_accel* accel) const
    {
      double firstDerivative = gsl_spline_eval_deriv(spline,refValue,accel) + (gsl_spline_eval_deriv2(spline,refValue,accel) * (value - refValue));
      double result = gsl_spline_eval(spline,refValue,accel) + (firstDerivative * (value - refValue));
      std::cout << "EXTRAPOLATE: " << refValue << " + " << firstDerivative << " * (" << value << " - " << refValue << ") = " << result << std::endl;
      return gsl_spline_eval(spline,refValue,accel) + (firstDerivative * (value - refValue));
    }
    
    double xToY(double x)
    {
      SharedLock lock(mutex);
      double y = 0.0;
      if (isInitialized && x <= this->xMax && x >= this->xMin)
	y = gsl_spline_eval (xToYSpline, x, xToYSplineAcc);
      else if (x > this->xMax)
	y = extrapolate(x,this->xMax,xToYSpline,xToYSplineAcc);
      else if (x < this->xMin)
	y = extrapolate(x,this->xMin,xToYSpline,xToYSplineAcc);
      else
	return NAN;
      if(y > yMaxLimit)
	return yMaxLimit;
      else if(y < yMinLimit)
	return yMinLimit;
      else
	return y;
    }
    
    double yToX(double y)
    {
      SharedLock lock(mutex);
      double x;
      if (isInitialized && y <= this->yMax && y >= this->yMin)
	x = gsl_spline_eval (yToXSpline, y, yToXSplineAcc);
      else if (y > this->yMax)
	x = extrapolate(y,this->yMax,yToXSpline,yToXSplineAcc);
      else if (y < this->yMin)
	x = extrapolate(y,this->yMin,yToXSpline,yToXSplineAcc);
      else
	return NAN;
      if(x > xMaxLimit)
	return xMaxLimit;
      else if(x < xMinLimit)
	return xMinLimit;
      else
	return x;
    }
    
  };
  
}//namespace lughos

#endif