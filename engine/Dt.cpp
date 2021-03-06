#include <engine/Dt.h>
#include <iostream>
using std::cout;
using std::endl;

int ydom[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

inline bool leap(int y) { return !(y % 400) || !(y % 4) && (y % 100); }

inline int diny(int y) { return 365 + (leap(y) ? 1 : 0); }

inline int dinm(int y, int m)
{
  return (!leap(y) || m != Feb) ? ydom[m + 1] - ydom[m] : 29;
}

inline int doy(int y, int m, int d)
{
  return ydom[m] + d + ((leap(y) && m > Feb) ? 1 : 0);
}

inline int nmult(int start, int end, int factor)
{
  return (end/factor*factor - start/factor*factor)/factor;
}

inline int nleap(int start, int end)
{
  return nmult(start, end, 4) - nmult(start, end, 100) + nmult(start, end, 400)
    + (leap(start) ? 1 : 0);
}

Err Dt::setDate(int y_, int m_, int d_)
{
  return change(y_, m_, --d_);
  if (y < 1900 || y > 2100)
    return invalid_yr;
  if (d < 0 || d >= dinm(y, m))
    return invalid_day;
  y = y_;
  m = m_;
  d = d_;
  return goodt;
}

Err Dt::setYear(int y_)
{
  return change(y_, m, d);
}

Err Dt::setMonth(int m_)
{
  return change(y, m_, d);
}

Err Dt::setDay(int d_)
{
  return change(y, m, --d_);
}

Err Dt::addDays(int n)
{
  int ynew = y, mnew = m, dnew = d;
  while (n != 0)
  {
    while (n > 0)
    {
      while (n >= diny(ynew) - doy(ynew, mnew, dnew))
        if (mnew < Feb)
          n -= diny(ynew++);
        else
          n -= diny(++ynew);
      while (n >= dinm(ynew, mnew) - dnew)
        n -= dinm(ynew, mnew++);
      if (n > 0)
      {
        dnew += n;
        n = 0;
      }
    }
    while (n < 0)
    {
      while (n < -doy(ynew, mnew, dnew))
        if (mnew < Feb)
          n += diny(--ynew);
        else
          n += diny(ynew--);
      while (n < -dnew)
        n += dinm(ynew, --mnew);
      if (n < 0)
      {
        dnew += n;
        n = 0;
      }
    }
  }

  return change(ynew, mnew, dnew);
}

Err Dt::addMonths(int n)
{
  int ynew = y, mnew = m;
  if (n >= 12 - mnew)
  {
    n += mnew;
    mnew = 0;
    ynew = y + n / 12;
    n %= 12;
  }
  else if (n < -mnew)
  {
    n += mnew - 11;
    mnew = 11;
    ynew = y - -n / 12;
    n = -(-n % 12);
  }
  mnew += n;

  return change(ynew, mnew, d);
}

Err Dt::addYears(int n)
{
  int ynew = y + n;

  return change(ynew, m, d);
}

int Dt::year() const { return y; }
int Dt::month() const { return m; }
int Dt::day() const { return d + 1; }

int Dt::weekday() const
{
  return (Mon + y-1900 + nleap(1900, m > Feb ? y : y-1) + ydom[m] + d) % 7;
}

bool Dt::operator<(const Dt& rval) const
{
  const Dt& lval = *this;
  if (lval.y < rval.y)
    return true;
  if (lval.y > rval.y)
    return false;
  if (lval.m < rval.m)
    return true;
  if (lval.m > rval.m)
    return false;
  if (lval.d < rval.d)
    return true;
  return false;
}

bool Dt::operator>(const Dt& rval) const
{
  const Dt& lval = *this;
  if (lval.y > rval.y)
    return true;
  if (lval.y < rval.y)
    return false;
  if (lval.m > rval.m)
    return true;
  if (lval.m < rval.m)
    return false;
  if (lval.d > rval.d)
    return true;
  return false;
}

bool Dt::operator<=(const Dt& rval) const
{
  const Dt& lval = *this;
  if (lval.y > rval.y)
    return false;
  if (lval.y < rval.y)
    return true;
  if (lval.m > rval.m)
    return false;
  if (lval.m < rval.m)
    return true;
  if (lval.d > rval.d)
    return false;
  return true;
}

bool Dt::operator>=(const Dt& rval) const
{
  const Dt& lval = *this;
  if (lval.y < rval.y)
    return false;
  if (lval.y > rval.y)
    return true;
  if (lval.m < rval.m)
    return false;
  if (lval.m > rval.m)
    return true;
  if (lval.d < rval.d)
    return false;
  return true;
}

bool Dt::operator==(const Dt& rval) const
{
  const Dt& lval = *this;
  return lval.y == rval.y && lval.m == rval.m && lval.d == rval.d;
}

bool Dt::operator!=(const Dt& rval) const
{
  const Dt& lval = *this;
  return lval.y != rval.y || lval.m != rval.m || lval.d != rval.d;
}

Err Dt::change(int y_, int m_, int d_)
{
  if (y_ < MIN_YEAR || y_ > MAX_YEAR)
    return invalid_yr;
  if (m_ > Dec)
    return invalid_mo;
  if (d_ < 0 || d_ >= dinm(y_, m_))
    return invalid_day;
  y = y_;
  m = m_;
  d = d_;
  return goodt;
}
