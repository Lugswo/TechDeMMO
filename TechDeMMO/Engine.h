#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <vector>

class Engine
{
public:
  static void Init();
  static void Update();
  static void Shutdown();

private:
  Engine() {};
  ~Engine() {};

    //  Settings file saved in this order:
    //    -  screen width
    //    -  screen height
  class Settings
  {
  public:
    Settings()
    {
      screenW = 1366;
      screenH = 768;

      ip = "192.168.1.puturiphere";
      user = "NONAME";
    }

    int screenW, screenH;
    std::string ip, user;
  };

  static Settings settings;

  static GLFWwindow *window;
  static bool successfullyOpened;

  static void SaveSettings();
  static bool LoadSettings();
};