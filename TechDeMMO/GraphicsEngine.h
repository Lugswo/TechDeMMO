#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

#include "Square.h"

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

  static bool LoadedCorrectly()
  {
    return loaded;
  }

  static Square & CreateSprite(const std::string &, unsigned &i, const std::string & shader = "Texture");

  static void RemoveSprite(unsigned i);

private:
  static void WindowResize(GLFWwindow *, int w, int h);
  static void GenerateFrameBuffer();

  static GLFWwindow *window;

  static GLuint fbo;
  static GLuint colorBuffer;

  static FrameBufferSquare *square;

  static int sWidth, sHeight;

  static bool loaded;

  static std::map<int, Square> sprites;

  static glm::mat4 proj;

  static Square *guy;

  static unsigned id;
};