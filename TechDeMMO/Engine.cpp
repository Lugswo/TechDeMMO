#include "Engine.h"

#include <iostream>
#include <fstream>
#include <glad.h>

#include "WindowManagement.h"
#include "InputManager.h"

#include "TraceLog.h"
#include "GraphicsEngine.h"
#include "FileUtils.h"
#include "Client.h"

GLFWwindow * Engine::window;
bool Engine::successfullyOpened;
Engine::Settings Engine::settings;

void Engine::SaveSettings()
{
  const char *mdpath = GetMyDocumentsPath();
  if (mdpath)
  {
    std::string str = mdpath;
    str += "/TechDeMMO/settings.ini";
    std::ofstream save(str);

    save << settings.screenW << std::endl;
    save << settings.screenH << std::endl;
    TraceLog::Log(TRACE_LEVEL::INFO, "Successfully saved!");
  }
  else
  {
    TraceLog::Log(TRACE_LEVEL::WARN, "Settings cannot be found!  Settings NOT saved!");
  }
}

bool Engine::LoadSettings()
{
  const char* mdpath = GetMyDocumentsPath();
  if (mdpath)
  {
    std::string str = mdpath;
    str += "/TechDeMMO/settings.ini";
    if (CheckForFile(str))
    {
      TraceLog::Log(TRACE_LEVEL::INFO, "Settings file successfully found.");
      std::ifstream file(str);
      file >> settings.screenW >> settings.screenH;

      TraceLog::Log(TRACE_LEVEL::INFO, "Settings file successfully loaded.");
    }
    else
    {
      TraceLog::Log(TRACE_LEVEL::WARN, "Settings file not found.  New one being generated.");

      std::ofstream file(str);
      if (file.bad())
      {
        TraceLog::Log(TRACE_LEVEL::FATAL, "Settings cannot be generated.  Unable to properly open game.");
        return false;
      }

      file << settings.screenW;
      file << settings.screenH;

      TraceLog::Log(TRACE_LEVEL::INFO, "Settings file successfully generated.");
    }
  }
  else
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Settings not found. Unable to properly open game.");
    return false;
  }

  return true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
    InputManager::PushKey(key);
  else if (action == GLFW_RELEASE)
    InputManager::RelKey(key);
}

void Engine::Init()
{
  successfullyOpened = false;

  TraceLog::Init();
  InputManager::Init();

  if (!LoadSettings())
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Something went wrong initializing the game.  Shutting down.");
    return;
  }

  GraphicsEngine::Init(settings.screenW, settings.screenH);
  window = GraphicsEngine::GetWindow();
  glfwSetKeyCallback(window, key_callback);

  if (!window)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Something went wrong initializing the game.  Shutting down.");
    return;
  }



  TraceLog::Log(TRACE_LEVEL::IMPORTANT, "Game successfully started.");
  successfullyOpened = true;
}

void Engine::Update()
{
  if (successfullyOpened)
  {
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();

      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      {
        glfwSetWindowShouldClose(window, true);
      }

      if (InputManager::KeyPress(GLFW_KEY_F7))
      {
        Client::Init();
      }

      GraphicsEngine::Update(1.f);

      glfwSwapBuffers(window);

        //  this goes last because it just refreshes all presses and releases
      InputManager::Update();
    }
  }

  return;
}

void Engine::Shutdown()
{
  SaveSettings();

  TraceLog::Log(TRACE_LEVEL::IMPORTANT, "Game succesfully closed.");
}
