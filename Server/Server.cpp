#include "Server.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "TraceLog.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT "25565"

bool Server::successfullyOpened = false;

void Server::Init(void)
{
  WSADATA wsa_data;

  int res;

  TraceLog::Init();

  // Initialize Winsock
  res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "WSA failed to start!  Error code " + std::to_string(res) + ".  Closing server now.");

    return;
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "WSA started successfully!");

  struct addrinfo *result = NULL, *ptr = NULL, hints;

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  res = getaddrinfo(nullptr, PORT, &hints, &result);

  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Getaddrinfo failed!  Error code " + std::to_string(res) + ".  Closing server.");

    return;
  }

  SOCKET sock = INVALID_SOCKET;
  sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

  if (sock == INVALID_SOCKET)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Socket creation failed!  Error code " + std::to_string(WSAGetLastError()) + ".  Closing server.");
    freeaddrinfo(result);

    return;
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "Socket create successfully!");

  res = bind(sock, result->ai_addr, static_cast<int>(result->ai_addrlen));
  if (res == SOCKET_ERROR)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Socket failed to bind!  Error code " + std::to_string(WSAGetLastError()) + ".  Closing server.");
    freeaddrinfo(result);

    return;
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "Socket bound successfully!");

  if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Failed to listen to socket!  Error code " + std::to_string(WSAGetLastError()) + ".  Closing server.");
    closesocket(sock);

    return;
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "Listening to socket successfully!");

  freeaddrinfo(result);

  successfullyOpened = true;
}

void Server::Update()
{
  while (true)
  {

  }
}

void Server::Shutdown(void)
{
  int res = WSACleanup();

  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "WSA failed to close!");
    return;
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "WSA shutdown successfully!");
}