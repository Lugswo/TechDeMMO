#include "WindowManagement.h"

#include <iostream>
#include <glad.h>

#include "TraceLog.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

GLFWwindow* CreateGLFWWindow(int width, int height)
{
  if (!glfwInit())
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "GLFW cannot initialize.  Game cannot open.");
    return nullptr;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(width, height, "TechDeMMO", NULL, NULL); //glfwGetPrimaryMonitor() 
  if (window == NULL)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Failed to create window.  Game cannot open.");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Glad failed to load.  Game cannot open properly.");
    return NULL;
  }

  glViewport(0, 0, width, height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  return window;
}
