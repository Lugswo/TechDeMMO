#include "PlayerComponent.h"

#include "ActionManager.h"
#include "InputManager.h"
#include "ActionTypes.h"

typedef ActionManager AM;
typedef ActionTypes AT;

const int speed = 10;

PlayerComponent::PlayerComponent(bool c)
{
  client = c;
}

void PlayerComponent::Init()
{
  trans = GetComponent(parent, TransformComponent);
}

void PlayerComponent::Update(float dt)
{
  if (client)
    InputLoop(dt);
  UpdateLoop(dt);
}

void PlayerComponent::InputLoop(float dt)
{
  if (InputManager::KeyDown(GLFW_KEY_A))
    am.Set(AT::L_MOVE);
  else
    am.Unset(AT::L_MOVE);

  if (InputManager::KeyDown(GLFW_KEY_W))
    am.Set(AT::U_MOVE);
  else
    am.Unset(AT::U_MOVE);

  if (InputManager::KeyDown(GLFW_KEY_S))
    am.Set(AT::D_MOVE);
  else
    am.Unset(AT::D_MOVE);

  if (InputManager::KeyDown(GLFW_KEY_D))
    am.Set(AT::R_MOVE);
  else
    am.Unset(AT::R_MOVE);
}

void PlayerComponent::MovePlayer(bool x, int s, float dt)
{
  if (x)
  {
    glm::vec3 p = trans->GetTranslation();
    p.x += s * dt;
    trans->SetTranslation(p);
  }
  else
  {
    glm::vec3 p = trans->GetTranslation();
    p.y += s * dt;
    trans->SetTranslation(p);
  }
}

void PlayerComponent::UpdateLoop(float dt)
{
  if (am.Check(AT::L_MOVE))
    MovePlayer(true, -speed, dt);
  if (am.Check(AT::R_MOVE))
    MovePlayer(true, speed, dt);
  if (am.Check(AT::D_MOVE))
    MovePlayer(false, -speed, dt);
  if (am.Check(AT::U_MOVE))
    MovePlayer(false, speed, dt);

  am.Update();
}