#include "Server.h"

#include <GLFW/glfw3.h>

#include "TraceLog.h"
#include "Packet.h"
#include "SharedVariables.h"

bool Server::successfullyOpened = false;
SOCKET Server::sock;
std::vector<Server::S_Client> Server::sockets;

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

void Server::CreateMessage(std::string &s, const std::string& user, const std::string& mess)
{
  s = "[";
  s += user;
  s += "] ";
  s += mess;
}

void Server::SendPacket(Packet &p, S_Client &cl, int i = -1)
{
  int res = send(cl.socket, p, p.GetSize(), 0);

  if (res == SOCKET_ERROR)
  {
    TraceLog::Log(TRACE_LEVEL::ERR, "Send failed!  Error code " + std::to_string(WSAGetLastError()) + ".  Disconnecting " + cl.user + ".");
    if (i != -1)
      sockets.erase(sockets.begin() + i);
    else
    {
      for (int i = 0; i < sockets.size(); ++i)
      {
        if (sockets[i].user == cl.user)
        {
          sockets.erase(sockets.begin() + i);
          break;
        }
      }
    }
  }
  else
    TraceLog::Log(TRACE_LEVEL::NETWORK, "Sent message '" + p.GetDesc() + "' to " + cl.user + ".");
}

void Server::SendPacketToAll(Packet &p)
{
  for (int i = 0; i < sockets.size(); ++i)
  {
    SendPacket(p, sockets[i], i);
  }
}

void Server::SendPacketToClient(Packet &p, int i)
{
  SendPacket(p, sockets[i]);
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
      res = select(sockets.back().socket + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);
    else
      res = select(sock + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);
    if (res > 0)
    {
      sockets.push_back(S_Client());

      S_Client *csock = &(sockets.back());
      csock->socket = accept(sock, nullptr, nullptr);

      if (csock->socket != INVALID_SOCKET)
      {
        //TraceLog::Log(TRACE_LEVEL::INFO, "Client found and accepted.");
        //res = send(csock->socket, sendMessage, 38, 0);
        //if (res == SOCKET_ERROR)
        //{
        //  TraceLog::Log(TRACE_LEVEL::ERR, "Send failed!  Error code " + std::to_string(WSAGetLastError()) + ".");
        //  sockets.pop_back();
        //}
        //else
        //{
        //  TraceLog::Log(TRACE_LEVEL::NETWORK, "Sent message \"" + str + "\" to client.");
          int timeout = 10;
          setsockopt(csock->socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
          setsockopt(csock->socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
        //}
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
      res = recv(sockets[i].socket, recbuf, 1024, 0);

      if (res > 0)
      {
        char *c = new char[1024];
        memcpy(c, recbuf, 1024);
        Packet p(c);

        PacketTypes t = p.GetType();

        switch (t)
        {
          case PacketTypes::TEXT:
          {
            std::string str = p.GetData();
            TraceLog::Log(TRACE_LEVEL::NETWORK, "Message '" + str + "' received.");

            std::string res;
            CreateMessage(res, sockets[i].user, str);

            Packet p(PacketTypes::TEXT, res, str);

            SendPacketToAll(p);
            break;
          }
          case PacketTypes::CSHUT:
          {
            std::string disconnectedMessage = sockets[i].user;
            disconnectedMessage += " disconnected.";
            TraceLog::Log(TRACE_LEVEL::NETWORK, disconnectedMessage);
            sockets.erase(sockets.begin() + i--);
            Packet p(PacketTypes::TEXT, disconnectedMessage, "Disconnect message.");
            SendPacketToAll(p);
            break;
          }
          case PacketTypes::LOGIN:
          {
            sockets[i].user = p.GetData();
            std::string str2 = sockets[i].user;
            str2 += " has joined.";
            TraceLog::Log(TRACE_LEVEL::NETWORK, str2);
            Packet pa(PacketTypes::TEXT, str2, "Login message.");
            SendPacketToAll(pa);
            break;
          }
          case PacketTypes::VERSION:
          {
            std::string str = p.GetData();
            if (str == version)
            {
              Packet pack(PacketTypes::TEXT, "Correct");
              SendPacketToClient(pack, i);
            }
          }
          default:
          {
            TraceLog::Log(TRACE_LEVEL::ERR, "Packet type unrecognized!");
            break;
          }
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