#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad.h>

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
  static void Restart();

private:
  static GLFWwindow *window;

  static GLuint fbo;
  static GLuint colorBuffers[2];

  //static FrameBufferSquare *square;
  //static FrameBufferSquare *lightSquare;

  static GLuint pingpongFBO[2];
  static GLuint pingpongBuffer[2];

  static int sWidth, sHeight;
};