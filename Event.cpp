#include "Event.h"
#include <cctype>
#include <sstream>
using std::istringstream;
using std::ostringstream;

bool parseDate(int& year, int& month, int& day, string date)
{
  string parser = date;
  for (string::iterator si = parser.begin(); si != parser.end(); ++si)
    if (isalpha(*si))
      *si = 'a';
    else if (isdigit(*si))
      *si = '0';
  if (parser != "aaa 00, 0000" && parser != "aaa 0, 0000")
    return false;

  for (string::iterator si = date.begin(); si != date.end(); ++si)
    *si = tolower(*si);

  istringstream streamdate(date);
  string smonth;
  streamdate >> smonth;
  if (smonth == "jan") month = Jan;
  else if (smonth == "feb") month = Feb;
  else if (smonth == "mar") month = Mar;
  else if (smonth == "apr") month = Apr;
  else if (smonth == "may") month = May;
  else if (smonth == "jun") month = Jun;
  else if (smonth == "jul") month = Jul;
  else if (smonth == "aug") month = Aug;
  else if (smonth == "sep") month = Sep;
  else if (smonth == "oct") month = Oct;
  else if (smonth == "nov") month = Nov;
  else if (smonth == "dec") month = Dec;
  else return false;

  streamdate >> day;

  char junk;
  streamdate.get(junk);
  streamdate.get(junk);

  streamdate >> year;

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

Event::Event(Cal* pcal_) : pcal(pcal_) {}

vector<string> Event::read(istream& in)
{
  string token;
  vector<string> errors;
  Dt begin, end, dt;
  begin.dtSet(1900, Jan, 1);
  end.dtSet(2100, Dec, 31);
  while (in >> token)
  {
    if (token == "Title")
      readTitle(errors, t, in);
    else if (token == "Notes")
      readNotes(errors, n, in);
    else if (token == "Occurrence")
    {
      readBlankLine(errors, in);
      while (in >> token, token != "EndOccurrence")
      {
        if (!in)
          errors.push_back("No \"EndOccurrence\" token found");
        else if (token == "On")
          readDate(errors, dt, in);
        else if (token == "Begin")
          readDate(errors, begin, in);
        else if (token == "End")
          readDate(errors, end, in);
        else if (token == "set")
        {
          readBlankLine(errors, in);
          if (dt != Dt())
            dts.push_back(dt);
          else
            errors.push_back(string("Token \"set\" found without date"));
        }
        else
          readInvalid(errors, token, in);
      }
    }
    else
      readInvalid(errors, token, in);
  }
  if (pcal)
    for (vector<Dt>::iterator di = dts.begin(); di != dts.end(); ++di)
      pcal->add(*di, this);
  return errors;
}

string Event::title() const
{
  return t;
}

string Event::notes() const
{
  return n;
}

vector<Dt> Event::dates() const
{
  return dts;
}
