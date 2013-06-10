#ifndef DT_H
#define DT_H
enum Func
{
  goodf = 0, dt_set
};

enum Type
{
  goodt = 0, invalid_yr, invalid_day
};

const int Jan = 0, Feb = 1, Mar = 2, Apr = 3, May = 4, Jun = 5, Jul = 6,
          Aug = 7, Sep = 8, Oct = 9, Nov = 10, Dec = 11;

const int Sun = 0, Mon = 1, Tue = 2, Wed = 3, Thu = 4, Fri = 5, Sat = 6;

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
  Err dtSet(int y_, int m_, int d_);
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
