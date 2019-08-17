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

  NAME(PlayerComponent)

private:
  void InputLoop(float dt);
  void UpdateLoop(float dt);

  void MovePlayer(bool, int s, float);

  bool client;

  TransformComponent *trans;

  ActionManager am;
};