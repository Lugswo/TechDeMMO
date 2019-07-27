#include "Engine.h"

#include <iostream>
#include <fstream>
#include <glad.h>
#include <filesystem>
#include <chrono>
#include <thread>

#include "WindowManagement.h"
#include "InputManager.h"

#include "TraceLog.h"
#include "GraphicsEngine.h"
#include "FileUtils.h"
#include "Client.h"

#include "SharedVariables.h"

#include "Shader.h"

GLFWwindow * Engine::window;
bool Engine::successfullyOpened;
Engine::Settings Engine::settings;
int Engine::Settings::screenH, Engine::Settings::screenW;
std::string Engine::Settings::ip, Engine::Settings::user;

std::chrono::steady_clock Engine::clock;
std::chrono::time_point<std::chrono::steady_clock> Engine::curr, Engine::previous, Engine::wait, Engine::wait2;
std::chrono::duration<double, std::nano> Engine::dt, Engine::counter;

static const int FRAME_RATE = 144;
static const double FRAME_TIME = (1 / (double)FRAME_RATE) * 1000000000;
static double FPS;

void Engine::SaveSettings()
{
  const std::string& mdpath = GetMyDocumentsPath();
  if (mdpath != "")
  {
    std::string str = mdpath;
    str += "/TechDeMMO/settings.ini";
    std::ofstream save(str);

    save << settings.screenW << std::endl;
    save << settings.screenH << std::endl;
    save << settings.ip << std::endl;
    save << settings.user << std::endl;
    TraceLog::Log(TRACE_LEVEL::INFO, "Successfully saved!");
  }
  else
  {
    TraceLog::Log(TRACE_LEVEL::WARN, "Settings cannot be found!  Settings NOT saved!");
  }
}

bool Engine::LoadSettings()
{
  const std::string& mdpath = GetMyDocumentsPath();
  if (mdpath != "")
  {
    std::string str = mdpath;
    std::string folderstr = mdpath;
    folderstr += "/TechDeMMO/";
    str += "/TechDeMMO/settings.ini";
    if (CheckForFile(str))
    {
      TraceLog::Log(TRACE_LEVEL::INFO, "Settings file successfully found.");
      std::ifstream file(str);
      file >> settings.screenW >> settings.screenH >> settings.ip >> settings.user;

      TraceLog::Log(TRACE_LEVEL::INFO, "Settings file successfully loaded.");
    }
    else
    {
      TraceLog::Log(TRACE_LEVEL::WARN, "Settings file not found.  New one being generated.");

      std::experimental::filesystem::create_directory(folderstr);

      std::ofstream file(str);
      if (file.bad())
      {
        TraceLog::Log(TRACE_LEVEL::FATAL, "Settings cannot be generated.  Unable to properly open game.");
        return false;
      }

      file << settings.screenW << std::endl;
      file << settings.screenH << std::endl;
      file << "192.168.puturshittyipherelol" << std::endl;
      file << "NONAME" << std::endl;

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

  if (!GraphicsEngine::LoadedCorrectly())
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Something went wrong initializing the graphics engine.  Shutting down.");
    return;
  }

  window = GraphicsEngine::GetWindow();
  glfwSetKeyCallback(window, key_callback);

  if (!window)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Something went wrong initializing the game.  Shutting down.");
    return;
  }

  Client::Init(settings.ip);
  successfullyOpened = true;

  TraceLog::Log(TRACE_LEVEL::IMPORTANT, "Game successfully started.");
}

void Engine::CloseWindow()
{
  glfwSetWindowShouldClose(window, true);
}

void Engine::Update()
{
  if (successfullyOpened)
  {
    while (!glfwWindowShouldClose(window))
    {
      // framerate limiter
      curr = clock.now();
      dt = curr - previous;

      if (dt.count() < FRAME_TIME)
      {
        std::chrono::duration<double, std::nano> delta_ms(FRAME_TIME - dt.count() - 1800000);
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(delta_ms);
        std::this_thread::sleep_for(std::chrono::nanoseconds(dur.count()));

        wait = clock.now();
        counter = wait - curr;

        if (counter.count() < FRAME_TIME)
        {
          std::chrono::duration<double, std::nano> delta_ms2(FRAME_TIME - dt.count() - counter.count());
          auto dur2 = std::chrono::duration_cast<std::chrono::nanoseconds>(delta_ms2);

          wait2 = clock.now();
          counter = wait2 - wait;

          while (counter.count() < dur2.count())
          {
            wait2 = clock.now();
            counter = wait2 - wait;
          }
        }
      }

      previous = clock.now();
      std::chrono::duration<double, std::nano> sleepTime = previous - curr;

      glfwPollEvents();

      Client::Update();
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

  Client::Shutdown();
}
