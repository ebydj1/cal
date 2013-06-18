#include <engine/Cal.h>

vector<Event*> Cal::getEvents(Dt dt) const
{
  if (days.find(dt) != days.end())
    return days.at(dt);
  else return vector<Event*>();
}
