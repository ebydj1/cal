#ifndef DT_H
#define DT_H
#include "Constants.h"

enum Func
{
  goodf = 0, dt_set, add_days, add_months, add_years
};

enum Type
{
  goodt = 0, invalid_yr, invalid_day, overflow, underflow
};

struct Err
{
  Func func;
  Type type;
  Err(Func func_, Type type_) : func(func_), type(type_) {}
  Err() : func(goodf), type(goodt) {}
};

class Dt
{
public:
  Dt() : y(0), m(0), d(0) {}
  Err dtSet(int y_, int m_, int d_);
  Err addDays(int);
  Err addMonths(int);
  Err addYears(int);

  int year() const;
  int month() const;
  int day() const;
  int weekday() const;

  bool operator<(const Dt&) const;
  bool operator>(const Dt&) const;
  bool operator<=(const Dt&) const;
  bool operator>=(const Dt&) const;
  bool operator==(const Dt&) const;
  bool operator!=(const Dt&) const;
private:
  int y;
  int m;
  int d;
};
#endif
