#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <chrono>
#include <vector>

#include "Packet.h"

#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
  static void Init(const std::string &ip);
  static void Update();
  static void Shutdown();

  static void ReceivePacket();
  static void SendPacket(Packet &p);

  static void InputText();

private:
  static bool connected, ping, chChange;
  static SOCKET sock;

  static std::chrono::steady_clock clock;
  static std::chrono::time_point<std::chrono::steady_clock> curr, prev;
  static std::chrono::duration<double, std::milli> time;

  static std::vector<Packet> packets;
};