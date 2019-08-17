#include "GameObject.h"

#include "TraceLog.h"
#include "Component.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
  int size = (int)components.size();
  for (int i = 0; i < size; ++i)
  {
    delete components[i];
  }
}

void GameObject::Update(float dt)
{
  int size = (int)components.size();
  for (int i = 0; i < size; ++i)
  {
    components[i]->Update(dt);
  }
}

void GameObject::Init()
{
  int size = (int)components.size();
  for (int i = 0; i < size; ++i)
  {
    components[i]->SetParent(this);
    components[i]->Init();
  }

  TraceLog::Log(TRACE_LEVEL::VERBOSE, "GameObject " + name + " created.");
}

const Component * GameObject::FindComponent(const std::string & name) const
{
  int size = (int)components.size();
  for (int i = 0; i < size; ++i)
  {
    if (components[i]->GetName() == name)
    {
      return components[i];
    }
  }
  TraceLog::Log(TRACE_LEVEL::ERR, "Error!  Component " + name + " not found in GameObject " + this->name + "!");
  return nullptr;
}

void GameObject::AddComponent(Component *c)
{
  components.push_back(c);
}

void GameObject::SetID(unsigned i)
{
  id = i;
}