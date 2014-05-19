#include "math.hpp"
#include <cmath>

double Math::GetDeltaPhi(const double phi1, const double phi2){
  return atan2(sin(phi2-phi1), cos(phi2-phi1));
}

double Math::GetAbsDeltaPhi(const double phi1, const double phi2){
  return fabs(GetDeltaPhi(phi1, phi2));
}

double Math::GetDeltaR(const double phi1, const double eta1, const double phi2, const double eta2){
  const double dPhi(GetAbsDeltaPhi(phi1,phi2));
  const double dEta(eta1-eta2);
  return std::sqrt(dPhi*dPhi+dEta*dEta);
}

double Math::CalcMT(const double px1, const double py1, const double px2, const double py2){
  return sqrt(2.0*(add_in_quadrature(px1, py1)*add_in_quadrature(px2, py2)-px1*px2-py1*py2));
}
