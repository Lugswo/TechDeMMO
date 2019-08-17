#pragma once

#include <vector>
#include <GLFW/glfw3.h>

class InputManager
{
public:
  static void Init();
  static void Update();

  static bool KeyPress(int key);
  static bool KeyPress(int key, int key2);
  static bool KeyDown(int key);
  static bool KeyDown(int key, int key2);
  static bool KeyUp(int key);
  static bool KeyUp(int key, int key2);
  static bool KeyRelease(int key);
  static bool KeyRelease(int key, int key2);

  static void PushKey(int key);
  static void RelKey(int key);

private:
  class Key
  {
  public:
    Key()
    {
      r = d = p = false;
      u = true;
    }
    bool r, p, u, d;
  };

  InputManager() {};
  ~InputManager() {};

  static Key keyboard[GLFW_KEY_MENU];
  static Key mouse[GLFW_MOUSE_BUTTON_8];

  static std::vector<Key *> keysToUpdate;
};