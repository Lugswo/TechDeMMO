#pragma once

#include <map>
#include <glm/glm.hpp>

#include "GameObject.h"

class GameObjectFactory
{
public:
  ~GameObjectFactory();
  static void Init();
  static void Update(float dt);

  static void CreatePlayer(bool t, glm::vec2 pos, unsigned id);
  static void DeletePlayer(unsigned i);

  static void AddObject(GameObject *);
  static void RemoveObject(unsigned);

private:
  GameObjectFactory();

  static void AddPlayer(GameObject *, unsigned);

  static std::map<unsigned, GameObject *> objectList;
  static std::map<unsigned, GameObject *> archetypeList;

  static std::map<unsigned, GameObject *> playerList;

  static unsigned id;
};