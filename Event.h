#include <istream>
#include <string>
#include <vector>
using std::istream;
using std::string;
using std::vector;
#include "Dt.h"

class Event
{
public:
  vector<string> read(istream&);

  string title();
  string notes();
  Dt     date();
private:
  string t;
  string n;
  Dt     dt;
};
