#include "InputManager.h"

#include <GLFW/glfw3.h>

#include "TraceLog.h"

InputManager::Key InputManager::keyboard[GLFW_KEY_MENU];
InputManager::Key InputManager::mouse[GLFW_MOUSE_BUTTON_8];
std::vector<InputManager::Key *> InputManager::keysToUpdate;

void InputManager::Init()
{
  for (int i = 0; i < GLFW_KEY_MENU; ++i)
  {
    keyboard[i] = Key();
  }

  for (int i = 0; i < GLFW_MOUSE_BUTTON_8; ++i)
  {
    mouse[i] = Key();
  }

  TraceLog::Log(TRACE_LEVEL::INFO, "Input manager successfully initiated.");
}

void InputManager::Update()
{
  int size = static_cast<int>(keysToUpdate.size());
  for (int i = 0; i < size; ++i)
  {
    keysToUpdate[i]->p = keysToUpdate[i]->r = false;
  }
  keysToUpdate.clear();
}

bool InputManager::KeyPress(int key)
{
  return keyboard[key].p;
}

//bool InputManager::KeyPress(int key, int key2)
//{
//
//}

bool InputManager::KeyDown(int key)
{
  return keyboard[key].d;
}

//bool InputManager::KeyDown(int key, int key2)
//{
//
//}
//
//bool InputManager::KeyUp(int key)
//{
//
//}
//
//bool InputManager::KeyUp(int key, int key2)
//{
//
//}
//
//bool InputManager::KeyRelease(int key)
//{
//
//}
//
//bool InputManager::KeyRelease(int key, int key2)
//{
//
//}

void InputManager::PushKey(int key)
{
  keyboard[key].p = keyboard[key].d = true;
  keyboard[key].u = false;
  keysToUpdate.push_back(keyboard + key);
}

void InputManager::RelKey(int key)
{
  keyboard[key].r = keyboard[key].u = true;
  keyboard[key].d = false;
  keysToUpdate.push_back(keyboard + key);
}