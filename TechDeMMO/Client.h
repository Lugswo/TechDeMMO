#pragma once

class Client
{
public:
  static void Init();
  static void Update();
  static void Shutdown();

private:
  static bool connected;
};