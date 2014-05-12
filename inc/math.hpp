#ifndef H_MATH
#define H_MATH

#include <cmath>
#include <vector>
#include <iterator>
#include <algorithm>

namespace Math{
  const double pi(4.0*atan(1.0));

  double GetDeltaPhi(const double, const double);
  double GetAbsDeltaPhi(const double, const double);
  double GetDeltaR(const double, const double, const double, const double);

  template<typename T>
  T add_in_quadrature(T x, T y){
    if(x<0) x=-x;
    if(y<0) y=-y;
    if(x>y && x>0){
      const T ratio(y/x);
      return x*sqrt(1.0+ratio*ratio);
    }else if(y>x && y>0.0){
      const T ratio(x/y);
      return y*sqrt(1.0+ratio*ratio);
    }else{
      return 0.0;
    }
  }

  template<typename T>
  typename T::value_type Sum(T begin, T end){
    //Performs Kahan summation (more precise than naive summation for long lists of numbers)
    typename T::value_type sum(0.0);
    volatile typename T::value_type correction(0.0);
    for(; begin!=end; ++begin){
      const typename T::value_type corrected_val(*begin-correction);
      const typename T::value_type temp_sum(sum+corrected_val);
      correction=(temp_sum-sum)-corrected_val;
      sum=temp_sum;
    }
    return sum;
  }

  template<typename T>
  typename T::value_type Mean(T begin, T end){
    return Sum(begin, end)/static_cast<typename T::value_type>(std::distance(begin,end));
  }

  template<typename T>
  typename T::value_type Variance(T begin, T end){
    const typename T::value_type mean(Mean(begin, end));
    std::vector<typename T::value_type> residuals_squared(0);
    for(T it(begin); it!=end; ++it){
      const typename T::value_type residual(*it-mean);
      residuals_squared.push_back(residual*residual);
    }
    return Sum(residuals_squared.begin(), residuals_squared.end())/static_cast<typename T::value_type>(std::distance(begin,end));
  }

  template<typename T>
  typename T::value_type Median(T begin, T end){
    //N.B.: As implemented, this function may reorder elements in the input range.
    std::sort(begin, end);
    const typename T::difference_type separation(std::distance(begin, end));
    end=begin;
    std::advance(end, floor(0.5*separation));
    if(separation & 0x1ul){
      return *end;
    }else{
      std::advance(begin, floor(0.5*separation)-1);
      return 0.5*((*begin)+(*end));
    }
  }

  template<typename T>
  typename T::value_type HalfSampleMode(T begin, T end){
    //N.B.: As implemented, this function may reorder elements in the input range.
    std::sort(begin, end);
    const typename T::difference_type separation(std::distance(begin, end));
    if(separation<=1){
      return 0.5*((*begin)+(*end));
    }else{
      T inner_begin(begin), inner_end(begin);
      std::advance(inner_end, ceil(0.5*separation));
      typename T::value_type min_delta((*inner_end)-(*inner_begin));
      T min_begin(inner_begin), min_end(inner_end);
      ++inner_begin;
      ++inner_end;
      while(inner_end!=end){
        const typename T::value_type this_delta((*inner_end)-(*inner_begin));
        if(this_delta<min_delta){
          min_delta=this_delta;
          min_begin=inner_begin;
          min_end=inner_end;
        }
        ++inner_begin;
        ++inner_end;
      }
      return HalfSampleMode(min_begin, min_end);
    }
  }
}

#endif
