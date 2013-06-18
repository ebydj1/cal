#ifndef CAL_H
#define CAL_H
#include <engine/Dt.h>
#include <map>
#include <vector>
using std::map;
using std::vector;

class Event;

class Cal
{
public:
  template<typename T> void add(T);
  vector<Event*> getEvents(Dt) const;
private:
  map<Dt, vector<Event*> > days;
};

template<typename T>
void Cal::add(T ep)
{
  const vector<Dt>& dts = ep->dates();
  vector<Dt>::const_iterator di = dts.begin();
  for (vector<Dt>::const_iterator di = dts.begin(); di != dts.end(); ++di)
    days[*di].push_back(&*ep);
}

#endif
