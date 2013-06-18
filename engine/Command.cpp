#include <engine/Command.h>
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
  else if (type == Month)
    e = dttemp.addMonths(num);
  else if (type == Year)
    e = dttemp.addYears(num);
  else if (type == Wday)
    e = dttemp.addDays((num + 7 - dttemp.weekday()) % 7);
  if (e != goodt)
  {
    ostringstream ssoutput;
    ssoutput << "Command \"Next " << num << " ";
    if (type == Day)
      ssoutput << "day";
    else if (type == Wday)
      if (num == Sun)
        ssoutput << "Sun";
      else if (num == Mon)
        ssoutput << "Mon";
      else if (num == Tue)
        ssoutput << "Tue";
      else if (num == Wed)
        ssoutput << "Wed";
      else if (num == Thu)
        ssoutput << "Thu";
      else if (num == Fri)
        ssoutput << "Fri";
      else
        ssoutput << "Sat";
    else if (type == Month)
      ssoutput << "month";
    else if (type == Year)
      ssoutput << "year";
    ssoutput << "\" generated an invalid date";
    errors.push_back(ssoutput.str());
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
  else if (type == Month)
    e = dttemp.addMonths(-num);
  else if (type == Year)
    e = dttemp.addYears(-num);
  else if (type == Wday)
    e = dttemp.addDays(-((dttemp.weekday() + 7 - num) % 7));
  if (e != goodt)
  {
    ostringstream ssoutput;
    ssoutput << "Command \"Prev " << num << " ";
    if (type == Day)
      ssoutput << "day";
    else if (type == Wday)
      if (num == Sun)
        ssoutput << "Sun";
      else if (num == Mon)
        ssoutput << "Mon";
      else if (num == Tue)
        ssoutput << "Tue";
      else if (num == Wed)
        ssoutput << "Wed";
      else if (num == Thu)
        ssoutput << "Thu";
      else if (num == Fri)
        ssoutput << "Fri";
      else
        ssoutput << "Sat";
    else if (type == Month)
      ssoutput << "month";
    else if (type == Year)
      ssoutput << "year";
    ssoutput << "\" generated an invalid date";
    errors.push_back(ssoutput.str());
  }
  else state.dt = dttemp;
}

To::To(vector<string>& errors, istream& in)
{
  readTo(errors, num, type, in);
}

void To::execute(State& state, vector<Dt>& dts, vector<string>& errors)
{
  Err e;
  Dt dttemp = state.dt;
  if (type == Day)
    e = dttemp.setDay(num);
  else if (type == Month)
    e = dttemp.setMonth(num);
  else if (type == Year)
    e = dttemp.setYear(num);
  else if (type == Wday)
    e = dttemp.addDays(num - dttemp.weekday());
  if (e != goodt)
  {
    ostringstream ssoutput;
    ssoutput << "Command \"To ";
    if (type == Day || type == Year)
      ssoutput << num;
    else if (type == Wday)
      if (num == Sun)
        ssoutput << "Sun";
      else if (num == Mon)
        ssoutput << "Mon";
      else if (num == Tue)
        ssoutput << "Tue";
      else if (num == Wed)
        ssoutput << "Wed";
      else if (num == Thu)
        ssoutput << "Thu";
      else if (num == Fri)
        ssoutput << "Fri";
      else
        ssoutput << "Sat";
    else if (type == Month)
      if (num == Jan)
        ssoutput << "Jan";
      else if (num == Feb)
        ssoutput << "Feb";
      else if (num == Mar)
        ssoutput << "Mar";
      else if (num == Apr)
        ssoutput << "Apr";
      else if (num == May)
        ssoutput << "May";
      else if (num == Jun)
        ssoutput << "Jun";
      else if (num == Jul)
        ssoutput << "Jul";
      else if (num == Aug)
        ssoutput << "Aug";
      else if (num == Sep)
        ssoutput << "Sep";
      else if (num == Oct)
        ssoutput << "Oct";
      else if (num == Nov)
        ssoutput << "Nov";
      else
        ssoutput << "Dec";
    ssoutput << "\" generated an invalid date";
    errors.push_back(ssoutput.str());
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
