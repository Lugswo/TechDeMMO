#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>

class Engine
{
public:
  static void Init();
  static void Update();
  static void Shutdown();

  class Settings
  {
  public:
    Settings()
    {
      screenW = 1366;
      screenH = 768;

      ip = "47.144.140.166";
      user = "someone stupid";
    }

    static int screenW, screenH;
    static std::string ip, user;
  };

  static void CloseWindow();

private:
  Engine() {};
  ~Engine() {};

    //  Settings file saved in this order:
    //    -  screen width
    //    -  screen height

  static Settings settings;

  static GLFWwindow *window;
  static bool successfullyOpened;

  static void SaveSettings();
  static bool LoadSettings();

  static std::chrono::steady_clock clock;
  static std::chrono::time_point<std::chrono::steady_clock> curr, previous, wait, wait2;
  static std::chrono::duration<double, std::nano> dt, counter;
};