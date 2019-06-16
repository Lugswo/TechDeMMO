#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

#define PORT "25565"

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
  static SOCKET sock;

  static std::vector<SOCKET> sockets;
};