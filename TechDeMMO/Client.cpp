#include "Client.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "TraceLog.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT "25565"

bool Client::connected = false;

void Client::Init()
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

  res = getaddrinfo("192.168.56.1", PORT, &hints, &result);
  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::ERR, "Getaddrinfo failed!  Error code " + std::to_string(res) + ".  Cannot connect to server.");

    return;
  }

  SOCKET sock = INVALID_SOCKET;

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

  TraceLog::Log(TRACE_LEVEL::INFO, "Successfully connected to server!");
  connected = true;
}

void Client::Shutdown()
{
  WSACleanup();
}