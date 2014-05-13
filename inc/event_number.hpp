#ifndef H_EVENT_NUMBER
#define H_EVENT_NUMBER

class EventNumber{
public:
  explicit EventNumber(const int=0, const int=0, const int=0);

  int GetRunNumber() const;
  int GetEventNumber() const;
  int GetLumiSection() const;

  void SetFullNumber(const int, const int, const int);

  void SetRunNumber(const int);
  void SetEventNumber(const int);
  void SetLumiSection(const int);

  bool operator==(const EventNumber&) const;
  bool operator!=(const EventNumber&) const;
  bool operator<(const EventNumber&) const ;
  bool operator>(const EventNumber&) const ;
  bool operator<=(const EventNumber&) const;
  bool operator>=(const EventNumber&) const;

  void Print() const;
private:
  int run, event, lumi;
};

#endif
