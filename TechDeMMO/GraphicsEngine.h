#pragma once

#include <GLFW/glfw3.h>

class GraphicsEngine
{
public:
  ~GraphicsEngine();

  static GLFWwindow *GetWindow()
  {
    return window;
  }

  static void Init(int w, int h);
  static void Update(float dt);

private:
  static GLFWwindow *window;
};