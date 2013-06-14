#include "Command.h"
#include <sstream>
using std::istringstream;
using std::ostringstream;

On::On(vector<string>& errors, istream& in)
{
  readDate(errors, dt, in);
}

void On::execute(State& state, vector<Dt>& dts, vector<string>& errors)
{
  state.dt = dt;
}

Begin::Begin(vector<string>& errors, istream& in)
{
  readDate(errors, begin, in);
}

void Begin::execute(State& state, vector<Dt>& dts, vector<string>& errors)
{
  state.begin = begin;
}

End::End(vector<string>& errors, istream& in)
{
  readDate(errors, end, in);
}

void End::execute(State& state, vector<Dt>& dts, vector<string>& errors)
{
  state.end = end;
}

Next::Next(vector<string>& errors, istream& in)
{
  readRel(errors, num, type, in);
}

void Next::execute(State& state, vector<Dt>& dts, vector<string>& errors)
{
  Err e;
  Dt dttemp = state.dt;
  if (type == Day)
    e = dttemp.addDays(num);
  else if (type == Week)
    e = dttemp.addDays(num*7);
  else if (type == Month)
    e = dttemp.addMonths(num);
  else if (type == Year)
    e = dttemp.addYears(num);
  if (e.type == overflow)
  {
    string output;
    ostringstream ssoutput(output);
    ssoutput << "Command \"Next " << num << " ";
    if (type == Day)
      ssoutput << "day";
    else if (type == Week)
      ssoutput << "week";
    else if (type == Month)
      ssoutput << "month";
    else if (type == Year)
      ssoutput << "year";
    ssoutput << " generated an invalid date";
    errors.push_back(output);
  }
  else state.dt = dttemp;
}

Prev::Prev(vector<string>& errors, istream& in)
{
  readRel(errors, num, type, in);
}

void Prev::execute(State& state, vector<Dt>& dts, vector<string>& errors)
{
  Err e;
  Dt dttemp = state.dt;
  if (type == Day)
    e = dttemp.addDays(-num);
  else if (type == Week)
    e = dttemp.addDays(-num*7);
  else if (type == Month)
    e = dttemp.addMonths(-num);
  else if (type == Year)
    e = dttemp.addYears(-num);
  if (e.type == underflow)
  {
    string output;
    ostringstream ssoutput(output);
    ssoutput << "Command \"Prev " << num << " ";
    if (type == Day)
      ssoutput << "day";
    else if (type == Week)
      ssoutput << "week";
    else if (type == Month)
      ssoutput << "month";
    else if (type == Year)
      ssoutput << "year";
    ssoutput << " generated an invalid date";
    errors.push_back(output);
  }
  else state.dt = dttemp;
}

Set::Set(vector<string>& errors, istream& in)
{
  readBlankLine(errors, in);
}

void Set::execute(State& state, vector<Dt>& dts, vector<string>& errors)
{
  if (state.dt != Dt())
    dts.push_back(state.dt);
  else
    errors.push_back(string("Token \"Set\" found without date"));
}
