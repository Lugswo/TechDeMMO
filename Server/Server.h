#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#include "Packet.h"

#define PORT "25565"

class Server
{
public:
  static void Init();
  static void Update();
  static void Shutdown();

  static void SendPacketToAll(Packet &p);
  static void SendPacketToClient(Packet &p, int i);

private:
  Server() {};
  ~Server() {};

  class S_Client
  {
  public:
    S_Client()
    {
      socket = INVALID_SOCKET;
    }

    SOCKET socket;
    std::string user;
  };

  static bool successfullyOpened;
  static SOCKET sock;

  static std::vector<S_Client> sockets;
};