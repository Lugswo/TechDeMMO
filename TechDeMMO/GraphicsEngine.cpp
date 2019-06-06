#include "GraphicsEngine.h"

#include "WindowManagement.h"

#include "TraceLog.h"

GLFWwindow *GraphicsEngine::window;

void GraphicsEngine::Init(int w, int h)
{
  window = CreateGLFWWindow(w, h);
}

GraphicsEngine::~GraphicsEngine()
{

}


void GraphicsEngine::Update(float dt)
{
  glClearColor(.2f, .2f, .19607f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}