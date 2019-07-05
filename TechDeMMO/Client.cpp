#include "Client.h"

#include <thread>

#include "TraceLog.h"
#include "InputManager.h"
#include "Engine.h"

#define PORT "25565"

bool Client::connected = false;
SOCKET Client::sock;

static char buf[1024];

void Client::Init(const std::string &ip)
{
  WSADATA wsa_data;
  int res;
  res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "WSA failed to start!  Error code " + std::to_string(res) + ".  Closing server now.");

    return;
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "WSA started successfully!");

  struct addrinfo *result = nullptr, *ptr = nullptr, hints;

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  res = getaddrinfo(ip.c_str(), PORT, &hints, &result);
  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::ERR, "Getaddrinfo failed!  Error code " + std::to_string(res) + ".  Cannot connect to server.");

    return;
  }

  sock = INVALID_SOCKET;

  ptr = result;
  sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

  if (sock == INVALID_SOCKET)
  {
    TraceLog::Log(TRACE_LEVEL::ERR, "Error creating socket!  Error code " + std::to_string(WSAGetLastError()) + ".  Cannot connect to server.");
    freeaddrinfo(result);

    return;
  }

  res = connect(sock, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
  if (res == SOCKET_ERROR)
  {
    closesocket(sock);
    sock = INVALID_SOCKET;
  }

  if (ptr->ai_next)
  {
    res = connect(sock, ptr->ai_next->ai_addr, static_cast<int>(ptr->ai_next->ai_addrlen));

    if (res == SOCKET_ERROR)
    {
      closesocket(sock);
      sock = INVALID_SOCKET;
    }
  }

  freeaddrinfo(result);

  if (sock == INVALID_SOCKET)
  {
    TraceLog::Log(TRACE_LEVEL::ERR, "Error connecting to server!");
    return;
  }

  int timeout = 10;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
  setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));

  // get server message

  std::string str = "Lugswo";
  Packet login(PacketTypes::LOGIN, str, "Login.");
  SendPacket(login);

  TraceLog::Log(TRACE_LEVEL::INFO, "Successfully connected to server!");
  connected = true;

  std::thread t(InputText);
  t.detach();
}

void Client::InputText()
{
  while (true)
  {
    std::string str;
    std::getline(std::cin, str);

    Packet p(PacketTypes::TEXT, str, str);
    SendPacket(p);
  }
}

void Client::Update()
{
  ReceivePacket();

  if (InputManager::KeyPress(GLFW_KEY_4))
  {
    std::string str = "Hello server!";
    Packet p(PacketTypes::TEXT, str, "Hello server!");
    SendPacket(p);
  }
}

void Client::ReceivePacket()
{
  if (connected == true)
  {
    int res = 0;
    res = recv(sock, buf, 1024, 0);

    if (res > 0)
    {
      char *c = new char[1024];
      memcpy(c, buf, 1024);
      Packet p(c);

      PacketTypes t = p.GetType();

      switch (t)
      {
        case PacketTypes::TEXT:
        {
          TraceLog::Log(TRACE_LEVEL::NETWORK, p.GetData());
          break;
        }
      }
    }
  }
}

void Client::SendPacket(Packet &p)
{
  int res = send(sock, p, p.GetSize(), 0);

  if (res == SOCKET_ERROR)
    TraceLog::Log(TRACE_LEVEL::ERR, "Failed to send message '" + p.GetDesc() + "'.  Error code " + std::to_string(WSAGetLastError()) + ".");
  else
    TraceLog::Log(TRACE_LEVEL::NETWORK, "Sent message '" + p.GetDesc() + "'.");
}

void Client::Shutdown()
{
  Packet p(PacketTypes::CSHUT);
  SendPacket(p);
  int res = shutdown(sock, SD_SEND);

  if (res == SOCKET_ERROR)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Shutdown failed!  Error code " + std::to_string(WSAGetLastError()) + ".");
  }
  closesocket(sock);
  WSACleanup();

  TraceLog::Log(TRACE_LEVEL::INFO, "Client successfully shutdown.");
}