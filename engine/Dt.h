#ifndef DT_H
#define DT_H
#include <engine/Constants.h>

enum Err
{
  goodt = 0, invalid_yr, invalid_day
};

class Dt
{
public:
  Dt() : y(0), m(0), d(0) {}
  Err setDate(int y_, int m_, int d_);
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
  Err change(int y_, int m_, int d_);

  int y;
  int m;
  int d;
};
#endif
