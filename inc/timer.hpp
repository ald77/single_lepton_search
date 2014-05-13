#ifndef H_TIMER
#define H_TIMER

#include <ctime>

class Timer{
public:
  explicit Timer(const unsigned long=0);
  void Start();
  void SetNumIterations(const unsigned long);
  void Iterate();
  void PrintRemainingTime() const;
  double GetRemainingTime() const;
private:
  time_t startTime;
  unsigned long numIts, curIts;
};

#endif
