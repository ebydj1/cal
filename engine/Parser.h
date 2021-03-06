#ifndef PARSER_H
#define PARSER_H
#include <istream>
#include <string>
#include <vector>
#include <engine/Dt.h>
using std::istream;
using std::string;
using std::vector;

string parseHelp(string str);
int getWday(string str);
int getMonth(string str);
bool parseDate(int& year, int& month, int& day, string date);
bool parseRel(int& num, int& type, string rel);
bool parseTo(int& num, int& type, string to);
void readTitle(vector<string>& errors, string& title, istream& in);
void readNotes(vector<string>& errors, string& notes, istream& in);
void readDate(vector<string>& errors, Dt& date, istream& in);
void readRel(vector<string>& errors, int& num, int& type, istream& in);
void readTo(vector<string>& errors, int& num, int& type, istream& in);
void readInvalid(vector<string>& errors, string token, istream& in);
void readBlankLine(vector<string>& errors, istream& in);
#endif
