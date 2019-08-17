#include "Server.h"

#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

#include "TraceLog.h"
#include "Packet.h"
#include "SharedVariables.h"
#include "RandomEngine.h"

bool Server::successfullyOpened = false, Server::running;
SOCKET Server::sock;
std::vector<std::vector<Server::S_Client>> Server::channels;
//std::vector<Server::S_Client> Server::sockets;

std::chrono::steady_clock Server::clock;
std::chrono::time_point<std::chrono::steady_clock> Server::curr, Server::prev;
std::chrono::duration<double, std::milli> Server::dt, Server::counter;

unsigned Server::id;

const int channelNum = 5;

#define CURR channels[j][i]

void Server::Init(void)
{
  WSADATA wsa_data;

  int res;

  TraceLog::Init();
  RandomEngine::Init();

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

  TraceLog::Log(TRACE_LEVEL::INFO, "Initializing channels.");
  for (int i = 0; i < channelNum; ++i)
  {
    channels.push_back(std::vector<S_Client>());

    TraceLog::Log(TRACE_LEVEL::INFO, "Channel " + std::to_string(i) + " initialized.");
  }
  TraceLog::Log(TRACE_LEVEL::INFO, "Channels successfully initialized.");

  std::thread t(InputText);
  t.detach();

  successfullyOpened = true;
  running = true;

  TraceLog::Log(TRACE_LEVEL::INFO, "Server started successfully.");

  id = 0;
}

void Server::InputText()
{
  while (true)
  {
    std::string str;
    std::getline(std::cin, str);

    if (str.front() == '/')
    {
      if (str == "/exit")
      {
        running = false;
      }
      else if (str == "/list")
      {
        for (int i = 0; i < channels.size(); ++i)
        {
          std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
          std::cout << "List of players in channel " << i << std::endl;
          for (int j = 0; j < channels[i].size(); ++j)
          {
            std::cout << channels[i][j].user << std::endl;
          }
          std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
        }
      }
      else
      {
        std::cout << "Command not recognized." << std::endl;
      }
    }
    else if (str != "")
    {
      std::string msg = "[Server] " + str;
      Packet p(PacketTypes::TEXT, msg, str);
      SendPacketToAll(p);
    }
  }
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
    for (int i = 0; i < channels[cl.channel].size(); ++i)
    {
      if (channels[cl.channel][i].user == cl.user)
      {
        channels[cl.channel].erase(channels[cl.channel].begin() + i);
        break;
      }
    }
  }
  else
    TraceLog::Log(TRACE_LEVEL::NETWORK, "Sent message '" + p.GetDesc() + "' to " + cl.user + ".");
}

void Server::SendPacketToAll(Packet &p)
{
  for (int j = 0; j < channelNum; ++j)
  {
    for (int i = 0; i < channels[j].size(); ++i)
    {
      SendPacket(p, channels[j][i], i);
    }
  }
}

void Server::SendPacketToAllBut(Packet &p, S_Client &cl)
{
  for (int j = 0; j < channelNum; ++j)
  {
    for (int i = 0; i < channels[j].size(); ++i)
    {
      if (cl.socket != channels[j][i].socket)
        SendPacket(p, channels[j][i], i);
    }
  }
}

void Server::SendPacketToChannel(Packet &p, int ch)
{
  for (int i = 0; i < channels[ch].size(); ++i)
  {
    SendPacket(p, channels[ch][i], i);
  }
}

void Server::Update()
{
  int res;
  char recbuf[1024];

  struct timeval tv;
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(sock, &rfds);

  tv.tv_sec = 0;
  tv.tv_usec = 10;

  while (running)
  {
    curr = clock.now();

    FD_ZERO(&rfds);
    FD_SET(sock, &rfds);

    //if (!sockets.empty())
    //  res = select(sockets.back().socket + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);
    //else
    res = select(sock + 1, &rfds, (fd_set *)0, (fd_set *)0, &tv);
    if (res > 0)
    {
      channels[0].push_back(S_Client(++id));

      S_Client *csock = &(channels[0].back());
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
        channels[0].pop_back();
      }
    }

    for (int j = 0; j < channelNum; ++j)
    {
      for (int i = 0; i < channels[j].size(); ++i)
      {
        if (!channels[j].empty())
          res = recv(channels[j][i].socket, recbuf, 1024, 0);
        else
          res = 0;

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
              CreateMessage(res, channels[j][i].user, str);

              Packet p(PacketTypes::TEXT, res, str);

              SendPacketToChannel(p, j);
              break;
            }
            case PacketTypes::CSHUT:
            {
              std::string disconnectedMessage = channels[j][i].user;
              disconnectedMessage += " disconnected.";
              TraceLog::Log(TRACE_LEVEL::NETWORK, disconnectedMessage);
              Packet p(PacketTypes::P_DISC, disconnectedMessage, "Disconnect message.");
              p.AddItem(CURR.id);
              channels[j].erase(channels[j].begin() + i--);
              SendPacketToAll(p);
              break;
            }
            case PacketTypes::LOGIN:
            {
              channels[j][i].user = p.GetData();
              channels[j][i].channel = j;
              channels[j][i].position = glm::vec2(RandomEngine::Float(-2.f, 2.f), RandomEngine::Float(-2.f, 2.f));
              std::string str2 = channels[j][i].user;
              str2 += " has joined.";
              TraceLog::Log(TRACE_LEVEL::NETWORK, str2);
              TraceLog::Log(TRACE_LEVEL::INFO, "Player spawned at " + std::to_string(CURR.position.x) + ", " + std::to_string(CURR.position.y));
              Packet pa(PacketTypes::LOGIN, str2, "Login message.");
              pa.AddItem(CURR.position);
              pa.AddItem(CURR.id);
              
              SendPacketToAllBut(pa, channels[j][i]);

              Packet pa2(PacketTypes::INIT, CURR.position);
              pa2.AddItem(CURR.id);

              int s = channels[j].size() - 1;

              pa2.AddItem(s);

              for (int k = 0; k < channels[i].size(); ++k)
              {
                if (channels[j][k].user != CURR.user)
                {
                  pa2.AddItem(channels[j][i].position);
                  pa2.AddItem(CURR.id);
                }
              }

              SendPacket(pa2, channels[j][i]);
              break;
            }
            case PacketTypes::VERSION:
            {
              std::string str = p.GetData();
              if (str == version)
              {
                Packet pack(PacketTypes::TEXT, "Correct");
                SendPacket(pack, channels[j][i]);
              }
              break;
            }
            case PacketTypes::PING:
            {
              Packet p(PacketTypes::PING);
              SendPacket(p, channels[j][i]);
              break;
            }
            case PacketTypes::CH_CHECK:
            {
              Packet p(PacketTypes::CH_CHECK, channels[j][i].channel);
              SendPacket(p, channels[j][i]);
              break;
            }
            case PacketTypes::CH_CHANGE:
            {
              int channel = p.GetData<int>();
              channels[channel].push_back(channels[j][i]);
              channels[j].erase(channels[j].begin() + i);
              Packet p(PacketTypes::TEXT, "Successfully moved to channel " + std::to_string(channel) + "!");
              SendPacket(p, channels[channel].back());
              channels[channel].back().channel = channel;
              Packet p2(PacketTypes::TEXT, channels[channel].back().user + " has joined channel " + std::to_string(channel) + ".");
              SendPacketToChannel(p2, channel);
              --i;
              break;
            }
            default:
            {
              TraceLog::Log(TRACE_LEVEL::ERR, "Packet type unrecognized!");
              break;
            }
          }
        }
        else if (res == 0)
        {

        }
      }
    }

    prev = clock.now();
    dt += prev - curr;

    if (dt.count() > 300000)
    {
      Packet p(PacketTypes::C_CHECK);
      SendPacketToAll(p);

      dt = prev - curr;
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