#include "PlayerComponent.h"

#include "ActionManager.h"
#include "InputManager.h"
#include "ActionTypes.h"
#include "Packet.h"
#include "Client.h"

typedef ActionManager AM;
typedef ActionTypes AT;

const int speed = 10;

PlayerComponent::PlayerComponent(bool c)
{
  client = c;
  moved = false;
}

void PlayerComponent::Init()
{
  trans = GetComponent(parent, TransformComponent);
}

void PlayerComponent::ServerSetPos(const glm::vec2 & p)
{
  pos = p;
}

void PlayerComponent::Update(float dt)
{
  if (client)
    InputLoop(dt);
}

void PlayerComponent::ServerUpdate(float dt)
{
  UpdateLoop(dt);
}

void PlayerComponent::SetKey(ActionTypes a, bool b)
{
  am.Set(a, b);
}

void PlayerComponent::InputLoop(float dt)
{
  if (InputManager::KeyPress(GLFW_KEY_W))
  {
    Packet p(PacketTypes::MOVE, true, "Moving up.");
    p.AddItem(AT::U_MOVE);

    Client::SendPacket(p);
  }

  if (InputManager::KeyRelease(GLFW_KEY_W))
  {
    Packet p(PacketTypes::MOVE, false, "Stopping moving.");
    p.AddItem(AT::U_MOVE);

    Client::SendPacket(p);
  }

  if (InputManager::KeyPress(GLFW_KEY_A))
  {
    Packet p(PacketTypes::MOVE, true, "Moving left.");
    p.AddItem(AT::L_MOVE);

    Client::SendPacket(p);
  }

  if (InputManager::KeyRelease(GLFW_KEY_A))
  {
    Packet p(PacketTypes::MOVE, false, "Stopping moving.");
    p.AddItem(AT::L_MOVE);

    Client::SendPacket(p);
  }
  if (InputManager::KeyPress(GLFW_KEY_S))
  {
    Packet p(PacketTypes::MOVE, true, "Moving down.");
    p.AddItem(AT::D_MOVE);

    Client::SendPacket(p);
  }

  if (InputManager::KeyRelease(GLFW_KEY_S))
  {
    Packet p(PacketTypes::MOVE, false, "Stopping moving.");
    p.AddItem(AT::D_MOVE);

    Client::SendPacket(p);
  }
  if (InputManager::KeyPress(GLFW_KEY_D))
  {
    Packet p(PacketTypes::MOVE, true, "Moving right.");
    p.AddItem(AT::R_MOVE);

    Client::SendPacket(p);
  }

  if (InputManager::KeyRelease(GLFW_KEY_D))
  {
    Packet p(PacketTypes::MOVE, false, "Stopping moving.");
    p.AddItem(AT::R_MOVE);

    Client::SendPacket(p);
  }

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
  //if (x)
  //{
  //  glm::vec3 p = trans->GetTranslation();
  //  p.x += s * dt;
  //  trans->SetTranslation(p);
  //}
  //else
  //{
  //  glm::vec3 p = trans->GetTranslation();
  //  p.y += s * dt;
  //  trans->SetTranslation(p);
  //}

  if (x)
  {
    pos.x += s * dt;
  }
  else
  {
    pos.y += s * dt;
  }

  moved = true;
}

void PlayerComponent::UpdateLoop(float dt)
{
  moved = false;

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