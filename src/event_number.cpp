#include <cstdio>
#include "event_number.hpp"

void EventNumber::Print() const{
  printf("Event number: %15d, run %10d, lumiblock %10d\n", event, run, lumi);
}

EventNumber::EventNumber(const int runIn, const int eventIn, const int lumiIn): run(runIn), event(eventIn), lumi(lumiIn){
}

int EventNumber::GetRunNumber() const{
  return run;
}

int EventNumber::GetEventNumber() const{
  return event;
}

int EventNumber::GetLumiSection() const{
  return lumi;
}

void EventNumber::SetFullNumber(const int runIn, const int eventIn, const int lumiIn){
  run=runIn;
  event=eventIn;
  lumi=lumiIn;
}

void EventNumber::SetRunNumber(const int runIn){
  run=runIn;
}

void EventNumber::SetEventNumber(const int eventIn){
  event=eventIn;
}

bool EventNumber::operator==(const EventNumber &eventIn) const{
  return eventIn.run==run && eventIn.event==event && eventIn.lumi==lumi;
}

bool EventNumber::operator!=(const EventNumber &eventIn) const{
  return !(*this==eventIn);
}

bool EventNumber::operator>(const EventNumber &eventIn) const{
  if(eventIn.event<event){
    return true;
  }else if(eventIn.event==event){
    if(eventIn.run<run){
      return true;
    }else if(eventIn.run==run){
      if(eventIn.lumi<lumi){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    }
  }else{
    return false;
  }
}

bool EventNumber::operator<(const EventNumber &eventIn) const{
  if(eventIn.event>event){
    return true;
  }else if(eventIn.event==event){
    if(eventIn.run>run){
      return true;
    }else if(eventIn.run==run){
      if(eventIn.lumi>lumi){
        return true;
      }else{
        return false;
      }
    }else{
      return false;
    }
  }else{
    return false;
  }
}

bool EventNumber::operator<=(const EventNumber &eventIn) const{
  return *this<eventIn || *this==eventIn;
}

bool EventNumber::operator>=(const EventNumber &eventIn) const{
  return *this>eventIn || *this==eventIn;
}
