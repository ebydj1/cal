#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
#include <string>
#include <istream>
using std::vector;
using std::string;
using std::istream;
#include <engine/Dt.h>
#include <engine/Parser.h>

struct State
{
  Dt begin, end, dt;
};

class Command
{
public:
  virtual void execute(State&, vector<Dt>&, vector<string>&) = 0;
};

class On : public Command
{
public:
  On(vector<string>& errors, istream& in);
  void execute(State& state, vector<Dt>& dts, vector<string>& errors);
private:
  Dt dt;
};

class Begin : public Command
{
public:
  Begin(vector<string>& errors, istream& in);
  void execute(State& state, vector<Dt>& dts, vector<string>& errors);
private:
  Dt begin;
};

class End : public Command
{
public:
  End(vector<string>& errors, istream& in);
  void execute(State& state, vector<Dt>& dts, vector<string>& errors);
private:
  Dt end;
};

class Next : public Command
{
public:
  Next(vector<string>& errors, istream& in);
  void execute(State& state, vector<Dt>& dts, vector<string>& errors);
private:
  int num;
  int type;
};

class Prev : public Command
{
public:
  Prev(vector<string>& errors, istream& in);
  void execute(State& state, vector<Dt>& dts, vector<string>& errors);
private:
  int num;
  int type;
};

class Set : public Command
{
public:
  Set(vector<string>& errors, istream& in);
  void execute(State& state, vector<Dt>& dts, vector<string>& errors);
};

#endif
