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
    errors.push_back(string("Date ").append(sdate).append(" invalid"));
  else
  {
    Err e = date.dtSet(year, month, day);
    if (e.type == invalid_yr)
    {
      string syear;
      ostringstream os(syear);
      os << year;
      errors.push_back(string("Invalid year ").append(syear));
    }
    else if (e.type == invalid_day)
    {
      string sday;
      ostringstream os(sday);
      os << day;
      errors.push_back(string("Invalid day ").append(sday));
    }
  }
}

Event::Event(Cal* pcal_) : pcal(pcal_) {}

vector<string> Event::read(istream& in)
{
  string token;
  vector<string> errors;
  while (in >> token)
  {
    if (token == "Title")
      readTitle(errors, t, in);
    else if (token == "Notes")
      readNotes(errors, n, in);
    else if (token == "Occurrence")
    {
      getline(in, token);
      if (token != "")
        errors.push_back(string("Text \"").append(token).append(" ignored"));
      while (in >> token, token != "EndOccurrence")
      {
        if (!in)
          errors.push_back("No \"EndOccurrence\" token found");
        else if (token == "On")
          readDate(errors, dt, in);
      }
    }
    else
    {
      errors.push_back(string("Token \"").append(token).append("\" not recognized"));
      getline(in, token);
    }
  }
  if (pcal)
    pcal->add(dt, this);
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

Dt Event::date() const
{
  return dt;
}
