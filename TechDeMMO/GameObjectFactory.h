#pragma once

#include <vector>

#include "GameObject.h"

class GameObjectFactory
{
public:
  ~GameObjectFactory();
  void Init();
  void Update(float dt);

private:
  GameObjectFactory();
  std::vector<GameObject *> objectList;
  std::vector<GameObject *> archetypeList;
};