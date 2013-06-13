#ifndef EVENT_H
#define EVENT_H
#include <iostream>
#include <istream>
#include <string>
#include <vector>
using std::istream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
#include "Dt.h"

struct State
{
  Dt begin, end, dt;
};

class Command
{
public:
  virtual void execute(State&, vector<Dt>&, vector<string>&) = 0;
};

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
