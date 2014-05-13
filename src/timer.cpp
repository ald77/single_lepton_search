#include "timer.hpp"
#include <ctime>
#include <cstdio>
#include <cmath>

Timer::Timer(const unsigned long itsIn):
  startTime(0),
  numIts(itsIn),
  curIts(0){
}

void Timer::Start(){
  curIts=0;
  time(&startTime);
}

void Timer::SetNumIterations(const unsigned long itsIn){
  numIts=itsIn;
}

void Timer::Iterate(){
  ++curIts;
}

double Timer::GetRemainingTime() const{
  if(curIts==0){
    return 0.0;
  }else{
    time_t curTime;
    time(&curTime);
    return (difftime(curTime,startTime)*(numIts-curIts))/curIts;
  }
}

void Timer::PrintRemainingTime() const{
  int secs(static_cast<int>(floor(GetRemainingTime()+0.5)));
  time_t endtime;
  time(&endtime);
  endtime+=secs;
  const short hours(secs/3600);
  secs-=3600*hours;
  const short minutes(secs/60);
  secs-=60*minutes;
  printf("Iteration %16ld of %16ld. %4hd:",curIts,numIts,hours);
  if(minutes<10){
    printf("0");
  }
  printf("%hd:",minutes);
  if(secs<10){
    printf("0");
  }
  printf("%d remaining. Expected finish: %s",secs, ctime(&endtime));
  fflush(stdout);
}
