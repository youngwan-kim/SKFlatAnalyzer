#ifndef Event_h
#define Event_h

#include <iostream>
#include <vector>

class Event{
public:

  Event();
  ~Event();

  void SetMCweight(double genw);

  void SetTrigger(std::vector<std::string> HLT_TriggerName, std::vector<bool> HLT_TriggerFired);
  bool PassTrigger(std::string trig);
  bool PassTrigger(std::vector<std::string> trigs);

private:
  double j_MCweight;
  std::vector<std::string> j_HLT_TriggerName;
  std::vector<bool> j_HLT_TriggerFired;

};

#endif
