#include <engine/Event.h>
#include <engine/Cal.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <vector>
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;

int main()
{
  DIR* dir = opendir("./");
  dirent* entry;
  vector<string> files;
  while (entry = readdir(dir))
  {
    string name = entry->d_name;
    if (name.length() >= 4
     && entry->d_type == DT_REG
     && name.substr(name.length()-4) == ".cal")
      files.push_back(name);
  }

  vector<Event> events;
  events.reserve(files.size());
  for (vector<string>::iterator si = files.begin(); si != files.end(); ++si)
  {
    ifstream calfile;
    calfile.open(si->c_str());
    events.push_back(Event());
    Event& event = events.back();
    vector<string> errors;
    cout << *si << endl;
    errors = event.read(calfile);
    for (vector<string>::iterator vsi = errors.begin();
         vsi != errors.end();
         ++vsi)
      cout << *vsi << endl;
    cout << "Title: " << event.title() << endl;
    cout << "Notes: " << event.notes() << endl;
    calfile.close();
  }
  cout << "Files read" << endl;

  Cal cal;
  for (vector<Event>::iterator ei = events.begin(); ei != events.end(); ++ei)
    cal.add(ei);
  Dt dt;
  dt.dtSet(2013, Jun, 8);
  vector<Event*> pevents = cal.getEvents(dt);
  for (vector<Event*>::iterator epi = pevents.begin();
       epi != pevents.end();
       ++epi)
  {
    cout << "Title: " << (*epi)->title() << endl
         << "Notes: " << (*epi)->notes() << endl;
  }
  cout << "Jun 8 2013 events displayed" << endl;
}
