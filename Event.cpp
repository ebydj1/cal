#include "Event.h"
#include <cctype>
#include <sstream>
using std::istringstream;
using std::ostringstream;

string parseHelp(string str)
{
  string::iterator si = str.begin();
  while (si != str.end())
  {
    if (isalpha(*si))
    {
      *si++ = 'a';
      while (isalpha(*si))
        str.erase(si);
    }
    else if (isdigit(*si))
    {
      *si++ = '0';
      while (isdigit(*si))
        str.erase(si);
    }
    else if (isspace(*si))
    {
      *si++ = ' ';
      while (isspace(*si))
        str.erase(si);
    }
    else ++si;
  }
  return str;
}

bool parseDate(int& year, int& month, int& day, string date)
{
  string parseddate = parseHelp(date);
  if (parseddate != "a 0, 0")
    return false;

  istringstream ssdate(date);
  string smonth;
  ssdate >> smonth;
  if (smonth == "Jan") month = Jan;
  else if (smonth == "Feb") month = Feb;
  else if (smonth == "Mar") month = Mar;
  else if (smonth == "Apr") month = Apr;
  else if (smonth == "May") month = May;
  else if (smonth == "Jun") month = Jun;
  else if (smonth == "Jul") month = Jul;
  else if (smonth == "Aug") month = Aug;
  else if (smonth == "Sep") month = Sep;
  else if (smonth == "Oct") month = Oct;
  else if (smonth == "Nov") month = Nov;
  else if (smonth == "Dec") month = Dec;
  else return false;

  ssdate >> day;

  char junk;
  ssdate.get(junk);
  ssdate.get(junk);

  ssdate >> year;

  return true;
}

enum RelTypes
{
  Day, Week, Month, Year
};

bool parseRel(int& num, int& type, string rel)
{
  string parsedrel = parseHelp(rel);
  if (parsedrel != "0 a" && parsedrel != "a")
    return false;

  istringstream ssrel(rel);

  if (parsedrel == "0 a")
    ssrel >> num;
  else num = 1;

  string stype;
  ssrel >> stype;
  if (stype == "day" || stype == "days") type = Day;
  else if (stype == "week" || stype == "weeks") type = Week;
  else if (stype == "month" || stype == "months") type = Month;
  else if (stype == "year" || stype == "years") type = Year;
  else return false;

  return true;
}

void readTitle(vector<string>& errors, string& title, istream& in)
{
  char junk;
  in.get(junk);
  if (junk == '\n')
    in.putback(junk);
  else getline(in, title);
  return;
}

void readNotes(vector<string>& errors, string& notes, istream& in)
{
  string line;
  getline(in, line);
  if (line != "")
  {
    notes.append(line);
    notes.push_back('\n');
  }
  while (getline(in, line), line != "EndNotes")
  {
    if (!in)
    {
      errors.push_back("No \"EndNotes\" token found");
      return;
    }
    notes.append(line);
    notes.push_back('\n');
  }
  return;
}

void readDate(vector<string>& errors, Dt& date, istream& in)
{
  string sdate;
  getline(in, sdate);
  sdate = sdate.substr(1);
  int year, month, day;
  if (parseDate(year, month, day, sdate) == false)
    errors.push_back(string("Date ") + sdate + string(" invalid"));
  else
  {
    Err e = date.dtSet(year, month, day);
    if (e.type == invalid_yr)
    {
      string syear;
      ostringstream os(syear);
      os << year;
      errors.push_back(string("Invalid year ") + syear);
    }
    else if (e.type == invalid_day)
    {
      string sday;
      ostringstream os(sday);
      os << day;
      errors.push_back(string("Invalid day ") + sday);
    }
  }
}

void readRel(vector<string>& errors, int& num, int& type, istream& in)
{
  string srel;
  getline(in, srel);
  srel = srel.substr(1);
  if (parseRel(num, type, srel) == false)
    errors.push_back(string("Relative date ") + srel + string(" invalid"));
}

void readInvalid(vector<string>& errors, string token, istream& in)
{
  errors.push_back(string("Token \"") + token + string("\" not recognized"));
  getline(in, token);
}

void readBlankLine(vector<string>& errors, istream& in)
{
  string junk;
  getline(in, junk);
  if (junk != "")
    errors.push_back(string("Text \"") + junk.substr(1) + string("\" ignored"));
}

class On : public Command
{
public:
  On(vector<string>& errors, istream& in)
  {
    readDate(errors, dt, in);
  }
  void execute(State& state, vector<Dt>& dts, vector<string>& errors)
  {
    state.dt = dt;
  }
private:
  Dt dt;
};

class Begin : public Command
{
public:
  Begin(vector<string>& errors, istream& in)
  {
    readDate(errors, begin, in);
  }
  void execute(State& state, vector<Dt>& dts, vector<string>& errors)
  {
    state.begin = begin;
  }
private:
  Dt begin;
};

class End : public Command
{
public:
  End(vector<string>& errors, istream& in)
  {
    readDate(errors, end, in);
  }
  void execute(State& state, vector<Dt>& dts, vector<string>& errors)
  {
    state.end = end;
  }
private:
  Dt end;
};

class Next : public Command
{
public:
  Next(vector<string>& errors, istream& in)
  {
    readRel(errors, num, type, in);
  }
  void execute(State& state, vector<Dt>& dts, vector<string>& errors)
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
private:
  int num;
  int type;
};

class Prev : public Command
{
public:
  Prev(vector<string>& errors, istream& in)
  {
    readRel(errors, num, type, in);
  }
  void execute(State& state, vector<Dt>& dts, vector<string>& errors)
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
private:
  int num;
  int type;
};

class Set : public Command
{
public:
  Set(vector<string>& errors, istream& in)
  {
    readBlankLine(errors, in);
  }
  void execute(State& state, vector<Dt>& dts, vector<string>& errors)
  {
    if (state.dt != Dt())
      dts.push_back(state.dt);
    else
    {
      errors.push_back(string("Token \"Set\" found without date"));
    }
  }
};

vector<string> Event::read(istream& in)
{
  string token;
  vector<string> errors;
  State state;
  Dt dt;
  state.begin.dtSet(1900, Jan, 1);
  state.end.dtSet(2100, Dec, 31);
  while (in >> token)
  {
    if (token == "Title")
      readTitle(errors, t, in);
    else if (token == "Notes")
      readNotes(errors, n, in);
    else if (token == "Occurrence")
    {
      readBlankLine(errors, in);
      while (in >> token && token != "EndOccurrence")
      {
        if (!in)
          errors.push_back("No \"EndOccurrence\" token found");
        else if (token == "On")
          commands.push_back(new On(errors, in));
        else if (token == "Begin")
          commands.push_back(new Begin(errors, in));
        else if (token == "End")
          commands.push_back(new End(errors, in));
        else if (token == "Next")
          commands.push_back(new Next(errors, in));
        else if (token == "Prev")
          commands.push_back(new Prev(errors, in));
        else if (token == "Set")
          commands.push_back(new Set(errors, in));
        else
          readInvalid(errors, token, in);
      }
    }
    else
      readInvalid(errors, token, in);
    for (vector<Command*>::iterator ci = commands.begin();
         ci != commands.end();
         ++ci)
      (*ci)->execute(state, dts, errors);
  }
  return errors;
}

const string& Event::title() const
{
  return t;
}

const string& Event::notes() const
{
  return n;
}

const vector<Dt>& Event::dates() const
{
  return dts;
}
