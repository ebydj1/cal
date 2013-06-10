#include "Dt.h"
#include <iostream>
using std::cout;
using std::endl;

int doy[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

inline bool leap(int y) { return !(y % 400) || !(y % 4) && (y % 100); }

inline int diny(int year) { return 365 + (leap(year) ? 1 : 0); }

inline int dinm(int year, int month)
{
  return (!leap(year) || month != Feb) ? doy[month + 1] - doy[month] : 29;
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

Err Dt::dtSet(int y_, int m_, int d_)
{
  --d_;
  if (y_ < 1900 || y_ > 2100)
    return Err(dt_set, invalid_yr);
  if (d_ >= dinm(y_, m_) || d_ < 0)
    return Err(dt_set, invalid_day);
  y = y_;
  m = m_;
  d = d_;
  return Err();
}

int Dt::weekday() const
{
  return (Mon + y-1900 + nleap(1900, m > Feb ? y : y-1) + doy[m] + d) % 7;
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
