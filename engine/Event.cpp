#include <engine/Event.h>
#include <engine/Parser.h>

vector<string> Event::read(istream& in)
{
  string token;
  vector<string> errors;
  State state;
  Dt dt;
  state.begin.dtSet(1900, Jan, 1);
  state.end.dtSet(2100, Dec, 31);
  while (in >> token)
  {
    if (token == "Title")
      readTitle(errors, t, in);
    else if (token == "Notes")
      readNotes(errors, n, in);
    else if (token == "Occurrence")
    {
      readBlankLine(errors, in);
      while (in >> token && token != "EndOccurrence")
      {
        if (!in)
          errors.push_back("No \"EndOccurrence\" token found");
        else if (token == "On")
          commands.push_back(new On(errors, in));
        else if (token == "Begin")
          commands.push_back(new Begin(errors, in));
        else if (token == "End")
          commands.push_back(new End(errors, in));
        else if (token == "Next")
          commands.push_back(new Next(errors, in));
        else if (token == "Prev")
          commands.push_back(new Prev(errors, in));
        else if (token == "Set")
          commands.push_back(new Set(errors, in));
        else
          readInvalid(errors, token, in);
      }
    }
    else
      readInvalid(errors, token, in);
    for (vector<Command*>::iterator ci = commands.begin();
         ci != commands.end();
         ++ci)
      (*ci)->execute(state, dts, errors);
  }
  return errors;
}

const string& Event::title() const
{
  return t;
}

const string& Event::notes() const
{
  return n;
}

const vector<Dt>& Event::dates() const
{
  return dts;
}
