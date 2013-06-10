#include "Cal.h"

void Cal::add(Dt dt, Event* ep)
{
  days[dt].push_back(ep);
}

vector<Event*> Cal::getEvents(Dt dt) const
{
  if (days.find(dt) != days.end())
    return days.at(dt);
  else return vector<Event*>();
}
