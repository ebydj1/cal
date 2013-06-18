#ifndef EVENT_H
#define EVENT_H
#include <istream>
#include <string>
#include <vector>
using std::istream;
using std::string;
using std::vector;
#include <engine/Dt.h>
#include <engine/Command.h>

class Event
{
public:
  vector<string> read(istream&);

  const string&     title() const;
  const string&     notes() const;
  const vector<Dt>& dates() const;
private:
  string     t;
  string     n;
  vector<Dt> dts;

  vector<Command*> commands;
};

#endif
