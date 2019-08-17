#pragma once

class Component;

#include <string>
#include <vector>

#define GetComponent(obj, name) (name*)obj->FindComponent(#name)

class GameObject
{
public:
  GameObject();
  ~GameObject();

  void AddComponent(Component *);

  void Init();
  void Update(float dt);

  const Component * FindComponent(const std::string & name) const;

  unsigned GetID()
  {
    return id;
  }

  void SetID(unsigned);

private:
  std::vector<Component *> components;
  std::string name;

  unsigned id;
};
