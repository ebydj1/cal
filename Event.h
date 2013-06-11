#ifndef EVENT_H
#define EVENT_H
#include <istream>
#include <string>
#include <vector>
using std::istream;
using std::string;
using std::vector;
#include "Cal.h"
#include "Dt.h"

class Event
{
public:
  Event(Cal*);
  vector<string> read(istream&);

  string     title() const;
  string     notes() const;
  vector<Dt> dates() const;
private:
  Cal*   pcal;

  string     t;
  string     n;
  vector<Dt> dts;
};
#endif
