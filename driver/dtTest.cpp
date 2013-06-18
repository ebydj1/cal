#include <engine/Dt.h>
#include <iostream>
using namespace std;

int main()
{
  // error checking and weekday generation tests
  Dt d;
  Err e;
  int w;
  e = d.setDate(0, Jan, 0);
  if (e != invalid_yr)
    cerr << "no error when giving a bad year" << endl;
  e = d.setDate(1900, Jan, 0);
  if (e != invalid_day)
    cerr << "no error when giving a stupid day" << endl;
  e = d.setDate(1900, Mar, 32);
  if (e != invalid_day)
    cerr << "no error when giving a day too high" << endl;
  e = d.setDate(1900, Feb, 29);
  if (e != invalid_day)
    cerr << "no error when giving an invalid leap day" << endl;
  e = d.setDate(1904, Feb, 29);
  if (e != goodt)
    cerr << "error when giving a valid leap day" << endl;
  e = d.setDate(2000, Feb, 29);
  if (e != goodt)
    cerr << "error when giving a valid leap day on Y2K" << endl;
  e = d.setDate(1900, Jan, 1);
  if (e != goodt)
    cerr << "error when giving a valid day" << endl;
  w = d.weekday();
  if (w != Mon)
    cerr << "January 1, 1900 not indicated as a Monday" << endl;
  e = d.setDate(2013, Jun, 9);
  if (e != goodt)
    cerr << "error when giving a valid day" << endl;
  w = d.weekday();
  if (w != Sun)
    cerr << "June 9, 2013 not indicated as a Sunday" << endl;

  // comparison tests
  Dt d1, d2, d3, d4;
  d1.setDate(1999, Jun, 15);
  d2.setDate(2000, Jun, 15);
  d3.setDate(2000, Jul, 15);
  d4.setDate(2000, Jul, 16);

  if (d1 > d2 || d1 > d3 || d1 > d4 || d2 > d3 || d2 > d4 || d3 > d4
   || d1 >= d2 || d1 >= d3 || d1 >= d4 || d2 >= d3 || d2 >= d4 || d3 >= d4
   || d1 == d2 || d1 == d3 || d1 == d4 || d2 == d3 || d2 == d4 || d3 == d4
   || d1 != d1 || d2 != d2 || d3 != d3 || d4 != d4)
    cerr << "Comparison problems - 1" << endl;
  if (!(d1 < d2 && d1 < d3 && d1 < d4 && d2 < d3 && d2 < d4 && d3 < d4
     && d1 <= d2 && d1 <= d3 && d1 <= d4 && d2 <= d3 && d2 <= d4 && d3 <= d4
     && d1 != d2 && d1 != d3 && d1 != d4 && d2 != d3 && d2 != d4 && d3 != d4
     && d1 == d1 && d2 == d2 && d3 == d3 && d4 == d4))
    cerr << "Comparison problems - 2" << endl;

  // arithmetic tests
  d.setDate(2000, Jan, 1);
  d1.setDate(2000, Jan, 1);
  d2.setDate(1999, Dec, 31);
  d3.setDate(1999, Dec, 1);
  d4.setDate(1999, Jan, 1);
  d.addDays(-1);
  if (d != d2)
  {
    cout << "Subtracting a day from Jan 1, 2000 does not work properly" << endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(1999, Dec, 31);
  }
  d.addDays(1);
  if (d != d1)
  {
    cout << "Adding a day to Dec 31, 1999 does not work properly" << endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(2000, Jan, 1);
  }
  d.addMonths(-1);
  if (d != d3)
  {
    cout << "Subtracting a month from Jan 1, 2000 does not work properly" << endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(1999, Dec, 1);
  }
  d.addMonths(1);
  if (d != d1)
  {
    cout << "Adding a month to Dec 1, 1999 does not work properly" << endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(2000, Jan, 1);
  }
  d.addYears(-1);
  if (d != d4)
  {
    cout << "Subtracting a year from Jan 1, 2000 does not work properly" <<endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(1999, Jan, 1);
  }
  d.addYears(1);
  if (d != d1)
  {
    cout << "Adding a year to Jan 1, 1999 does not work properly" << endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(2000, Jan, 1);
  }
  d.setDate(2000, Feb, 28);
  d1.setDate(2000, Feb, 28);
  d2.setDate(2000, Feb, 29);
  d.addDays(1);
  if (d != d2)
  {
    cout << "Adding a day to Feb 28, 2000 does not work properly" << endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(2000, Feb, 29);
  }
  d.addDays(-1);
  if (d != d1)
  {
    cout << "Subtracting a day from Feb 29, 2000 does not work properly" << endl;
    cout << "Given: " << d.year() << " " << d.month() << " " << d.day() << endl;
    d.setDate(2000, Feb, 28);
  }
}
