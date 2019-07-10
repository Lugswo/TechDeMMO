#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <string>
#include <chrono>

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
    unsigned channel;
  };

  static void SendPacket(Packet &p, S_Client &cl, int i);

  static void CreateMessage(std::string &, const std::string&, const std::string&);

  static std::vector<std::vector<S_Client>> channels;
  //static std::vector<S_Client> sockets;

  static std::chrono::steady_clock clock;
  static std::chrono::time_point<std::chrono::steady_clock> curr, prev;
  static std::chrono::duration<double, std::milli> dt, counter;

  static void InputText();

  static bool successfullyOpened, running;
  static SOCKET sock;
};