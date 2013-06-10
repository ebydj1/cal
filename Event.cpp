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

Event::Event(Cal* pcal_) : pcal(pcal_) {}

vector<string> Event::read(istream& in)
{
  string token;
  vector<string> retval;
  while (in >> token)
  {
    if (token == "Title")
      getline(in, t);
    else if (token == "Notes")
    {
      getline(in, token);
      if (token != "")
      {
        n.append(token);
        n.push_back('\n');
      }
      while (getline(in, token), token != "End Notes")
      {
        n.append(token);
        n.push_back('\n');
        if (!in)
        {
          retval.push_back("No \"End Notes\" token found");
          goto endEventRead;
        }
      }
    }
    else if (token == "On")
    {
      getline(in, token);
      token = token.substr(1);
      int year, month, day;
      if (parseDate(year, month, day, token) == false)
        retval.push_back(string("Date ").append(token).append(" invalid"));
      else
      {
        Err e = dt.dtSet(year, month, day);
        if (e.type == invalid_yr)
        {
          string syear;
          ostringstream os(syear);
          os << year;
          retval.push_back(string("Invalid year ").append(syear));
        }
        else if (e.type == invalid_day)
        {
          string sday;
          ostringstream os(sday);
          os << day;
          retval.push_back(string("Invalid day ").append(sday));
        }
      }
    }
    else retval.push_back(string("Token \"").append(token).append("\" not recognized"));
  }
  if (pcal)
    pcal->add(dt, this);
endEventRead:
  return retval;
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
