#include "math.hpp"

double Math::GetDeltaPhi(const double phi1, const double phi2){
  const double dp(phi1-phi2);
  return (dp>0.0)?fmod(dp+pi,2.0*pi)-pi:fmod(dp-pi,2.0*pi)+pi;
}

double Math::GetAbsDeltaPhi(const double phi1, const double phi2){
  return fmod(fabs(phi1-phi2)+pi,2.0*pi)-pi;
}

double Math::GetDeltaR(const double phi1, const double eta1, const double phi2, const double eta2){
  const double dPhi(GetAbsDeltaPhi(phi1,phi2));
  const double dEta(eta1-eta2);
  return std::sqrt(dPhi*dPhi+dEta*dEta);
}
