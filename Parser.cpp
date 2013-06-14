#include "Parser.h"
#include "Constants.h"

#include <cctype>
#include <sstream>
using std::istringstream;
using std::ostringstream;

int getWday(string str)
{
  if (str == "Sun") return Sun;
  else if (str == "Mon") return Mon;
  else if (str == "Tue") return Tue;
  else if (str == "Wed") return Wed;
  else if (str == "Thu") return Thu;
  else if (str == "Fri") return Fri;
  else if (str == "Sat") return Sat;
  else return -1;
}

int getMonth(string str)
{
  if (str == "Jan") return Jan;
  else if (str == "Feb") return Feb;
  else if (str == "Mar") return Mar;
  else if (str == "Apr") return Apr;
  else if (str == "May") return May;
  else if (str == "Jun") return Jun;
  else if (str == "Jul") return Jul;
  else if (str == "Aug") return Aug;
  else if (str == "Sep") return Sep;
  else if (str == "Oct") return Oct;
  else if (str == "Nov") return Nov;
  else if (str == "Dec") return Dec;
  else return -1;
}

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
  month = getMonth(smonth);
  if (month == -1)
    return false;

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
  if (stype == "day") type = Day;
  else if (stype == "month") type = Month;
  else if (stype == "year") type = Year;
  else if (getWday(stype) != -1)
  {
    num = getWday(stype);
    type = Wday;
  }
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
  int numtemp, typetemp;
  string srel;
  getline(in, srel);
  srel = srel.substr(1);
  if (parseRel(numtemp, typetemp, srel) == false)
    errors.push_back(string("Relative date ") + srel + string(" invalid"));
  else
  {
    num = numtemp;
    type = typetemp;
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
