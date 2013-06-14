#include "Parser.h"
#include "Constants.h"

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
