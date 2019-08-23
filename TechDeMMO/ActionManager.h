#pragma once

#include <map>
#include <vector>

#include "ActionTypes.h"

class ActionManager
{
public:
  ActionManager();
  ~ActionManager();

  void Update();

  bool Check(ActionTypes a);
  void Set(ActionTypes a);
  void Unset(ActionTypes a);
  void FrameSet(ActionTypes a);

  void Set(ActionTypes a, bool b);

private:

  std::map<ActionTypes, bool> actions;
  std::vector<ActionTypes> frameActions;
};