#include "GameObjectFactory.h"

#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "PlayerComponent.h"
#include "RandomEngine.h"

std::map<unsigned, GameObject *> GameObjectFactory::archetypeList, GameObjectFactory::objectList, GameObjectFactory::playerList;
unsigned GameObjectFactory::id;

GameObjectFactory::~GameObjectFactory()
{
  for (auto itr = objectList.begin(); itr != objectList.end(); ++itr)
  {
    delete (*itr).second;
  }

  for (auto itr = archetypeList.begin(); itr != archetypeList.end(); ++itr)
  {
    delete (*itr).second;
  }
}

void GameObjectFactory::Init()
{
  id = 0;
}

void GameObjectFactory::Update(float dt)
{
  for (auto itr = objectList.begin(); itr != objectList.end(); ++itr)
  {
    (*itr).second->Update(dt);
  }
}

void GameObjectFactory::CreatePlayer(bool t, glm::vec2 pos, unsigned i)
{
  GameObject *obj = new GameObject();
  TransformComponent *trans = new TransformComponent();
  SpriteComponent *sprite = new SpriteComponent("ArtAssets/bub.png");
  PlayerComponent *player = new PlayerComponent(t);

  obj->AddComponent(trans);
  obj->AddComponent(sprite);
  obj->AddComponent(player);

  glm::vec3 a = trans->GetTranslation();
  a.x = pos.x;
  a.y = pos.y;
  trans->SetTranslation(a);
  
  AddPlayer(obj, i);
}

void GameObjectFactory::DeletePlayer(unsigned i)
{
  GameObject *temp = playerList[i];
  objectList.erase(temp->GetID());
  playerList.erase(i);

  delete temp;
}

void GameObjectFactory::AddObject(GameObject *obj)
{
  ++id;
  obj->SetID(id);
  objectList.insert(std::pair<unsigned, GameObject*>(id, obj));
  obj->Init();
}

void GameObjectFactory::AddPlayer(GameObject *obj, unsigned i)
{
  AddObject(obj);
  playerList.insert(std::pair<unsigned, GameObject *>(i, obj));
}

void GameObjectFactory::RemoveObject(unsigned i)
{
  objectList.erase(i);
}