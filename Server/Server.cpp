#include "Server.h"

#include <GLFW/glfw3.h>

#include <conio.h>

#include "TraceLog.h"
#include "Packet.h"

bool Server::successfullyOpened = false;
SOCKET Server::sock;
std::vector<SOCKET> Server::sockets;

static const char *sendMessage = "Successfully connected to the server.";
static const std::string str = "Successfully connected to the server.";

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

  sock = INVALID_SOCKET;
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
  int res;
  char recbuf[1024];

  while (true)
  {
    struct timeval tv;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);

    tv.tv_sec = 0;
    tv.tv_usec = 10;

    if (!sockets.empty())
      res = select(sockets.back() + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);
    else
      res = select(sock + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);
    if (res > 0)
    {
      sockets.push_back(SOCKET());

      SOCKET *csock = &(sockets.back());
      *csock = accept(sock, nullptr, nullptr);

      if (*csock != INVALID_SOCKET)
      {
        TraceLog::Log(TRACE_LEVEL::INFO, "Client found and accepted.");
        res = send(*csock, sendMessage, 38, 0);
        if (res == SOCKET_ERROR)
        {
          TraceLog::Log(TRACE_LEVEL::ERR, "Send failed!  Error code " + std::to_string(WSAGetLastError()) + ".");
          sockets.pop_back();
        }
        else
        {
          TraceLog::Log(TRACE_LEVEL::NETWORK, "Sent message \"" + str + "\" to client.");
          int timeout = 10;
          setsockopt(*csock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
          setsockopt(*csock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
        }
      }
      else
      {
        TraceLog::Log(TRACE_LEVEL::ERR, "Connection to client failed!  Error code " + std::to_string(WSAGetLastError()) + ".");
        sockets.pop_back();
      }
    }

    int size = static_cast<int>(sockets.size());
    for (int i = 0; i < size; ++i)
    {
      res = recv(sockets[i], recbuf, 1024, 0);

      if (res > 0)
      {
        Packet p(recbuf);
        std::string str = p.GetData();
        TraceLog::Log(TRACE_LEVEL::NETWORK, "Message '" + str + "' received.");
      }
    }

    if (_kbhit())
    {
      if (_getch() == 'e')
      {
        for (int i = 0; i < size; ++i)
        {
          res = send(sockets[i], "alexo suxo", 11, 0);

          if (res == SOCKET_ERROR)
            TraceLog::Log(TRACE_LEVEL::ERR, "Send failed!  Error code " + std::to_string(WSAGetLastError()) + ".");
          else
            TraceLog::Log(TRACE_LEVEL::NETWORK, "Sent message \"alexo suxo\" to client.");
        }
      }
    }
  }
}

void Server::Shutdown(void)
{
  int res;

  res = closesocket(sock);
  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "Failed to close socket!  Error code " + std::to_string(res) + ".  Closing server.");
  }

  res = WSACleanup();

  if (res != 0)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "WSA failed to close!");
    return;
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "WSA shutdown successfully!");
}