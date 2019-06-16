#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

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
  static bool connected;
  static SOCKET sock;
};