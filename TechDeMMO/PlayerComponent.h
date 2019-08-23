#pragma once

#include "Component.h"

#include "TransformComponent.h"
#include "ActionManager.h"

class PlayerComponent : public Component
{
public:
  PlayerComponent(bool);
  void Init() override;
  void Update(float dt) override;

  void ServerUpdate(float dt);

  void SetKey(ActionTypes a, bool b);

  void ServerSetPos(const glm::vec2 &);

  bool Moved() const
  {
    return moved;
  }

  const glm::vec2 GetPosition() const
  {
    return pos;
  }

  void SetPosition(const glm::vec2 & v)
  {
    pos = v;
  }

  NAME(PlayerComponent)

private:
  void InputLoop(float dt);
  void UpdateLoop(float dt);

  void MovePlayer(bool, int s, float);

  bool client, moved;

  glm::vec2 pos;

  TransformComponent *trans;

  ActionManager am;
};