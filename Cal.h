#ifndef CAL_H
#define CAL_H
#include "Dt.h"
#include <map>
#include <vector>
using std::map;
using std::vector;

class Event;

class Cal
{
public:
  void add(Dt, Event*);
  vector<Event*> getEvents(Dt) const;
private:
  map<Dt, vector<Event*> > days;
};
#endif
