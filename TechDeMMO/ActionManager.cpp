#include "ActionManager.h"

#include "TraceLog.h"

ActionManager::ActionManager()
{
  for (unsigned i = 0; i != (int)ActionTypes::END; ++i)
  {
    actions[(ActionTypes)i] = false;
  }

  TraceLog::Log(TRACE_LEVEL::VERBOSE, "ActionManager successfully initiated.");
}

ActionManager::~ActionManager()
{

}

void ActionManager::Update()
{
  int size = static_cast<int>(frameActions.size());
  for (int i = 0; i < size; ++i)
  {
    actions[frameActions[i]] = false;
  }
}

void ActionManager::Set(ActionTypes t)
{
  actions[t] = true;
}

void ActionManager::Set(ActionTypes t, bool b)
{
  actions[t] = b;
}

void ActionManager::FrameSet(ActionTypes t)
{
  actions[t] = true;
  frameActions.push_back(t);
}

void ActionManager::Unset(ActionTypes t)
{
  actions[t] = false;
}

bool ActionManager::Check(ActionTypes t)
{
  return actions[t];
}
