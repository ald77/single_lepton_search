#ifndef H_WEIGHTCALCULATOR
#define H_WEIGHTCALCULATOR

#include <string>
#include <map>

class WeightCalculator{
public:
  explicit WeightCalculator(const double=19399);

  void SetLuminosity(const double lumiIn=19399);
  double GetLuminosity() const;

  double GetCrossSection(const std::string&) const;
  double GetCrossSection(const std::string&, const int, const int) const;

  int GetTotalEvents(const std::string&) const;
  int GetTotalEvents(const std::string&, const int, const int) const;

  double GetWeight(const std::string&) const;
  double GetWeight(const std::string&, const int, const int) const;

private:
  static std::map<std::string, double> crossSectionTable;
  static std::map<std::string, int> totalEventsTable;
  double lumi;

  void SetCrossSections();
  void SetTotalEvents();

  static int GetT1tttt14TeVTotalEvents(const int m1, const int m2);
  static double GetT1tttt14TeVCrossSection(const int m1, const int m2);
};

#endif
