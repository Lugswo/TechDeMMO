#pragma once

class Server
{
public:
  static void Init();
  static void Update();
  static void Shutdown();

private:
  Server() {};
  ~Server() {};

  static bool successfullyOpened;
};