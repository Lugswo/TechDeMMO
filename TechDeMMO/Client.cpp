#include "Client.h"

#include <thread>

#include "TraceLog.h"
#include "InputManager.h"
#include "Engine.h"
#include "SharedVariables.h"
#include "TransformComponent.h"

#include "GameObjectFactory.h"
#include "PlayerComponent.h"

#define PORT "25565"

bool Client::connected = false, Client::ping = false, Client::chChange = false;
SOCKET Client::sock;
std::chrono::steady_clock Client::clock;
std::chrono::time_point<std::chrono::steady_clock> Client::curr, Client::prev;
std::chrono::duration<double, std::milli> Client::time;
std::vector<Packet> Client::packets;

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


  // get server message

  std::string ver = version;
  Packet verCheck(PacketTypes::VERSION, ver, "Version check.");
  SendPacket(verCheck);

  res = recv(sock, buf, 1024, 0);

  if (res > 0)
  {
    unsigned size = *(reinterpret_cast<unsigned *>(buf));
    Packet p(buf, size);

    std::string ver2 = p.GetData();
    if (ver2 != "Correct")
    {
      TraceLog::Log(TRACE_LEVEL::FATAL, "Game not updated!  Please update the game in order to connect to the server.");
      return;
    }
    else
    {
      TraceLog::Log(TRACE_LEVEL::INFO, "Version verified successfully.");
    }
  }

  int timeout = 10;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
  setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));

  std::string str = Engine::Settings::user;
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
    if (!chChange)
    {
      std::string str;
      std::getline(std::cin, str);

      if (str.front() == '/')
      {
        if (str == "/exit")
        {
          Engine::CloseWindow();
        }
        else if (str == "/ping")
        {
          ping = true;
        }
        else if (str == "/change")
        {
          Packet check(PacketTypes::CH_CHECK);
          SendPacket(check);
          chChange = true;
        }
        else
        {
          std::cout << "Command not recognized." << std::endl;
        }
      }
      else if (str != "")
      {
        Packet p(PacketTypes::TEXT, str, str);
        SendPacket(p);
      }
    }
  }
}

void Client::Update()
{
  if (ping)
  {
    Packet p(PacketTypes::PING);
    SendPacket(p);
    prev = clock.now();
    ping = false;
  }

  if (InputManager::KeyPress(GLFW_KEY_4))
  {
    Packet p(PacketTypes::TEXT, "FRICK");
    SendPacket(p);
    SendPacket(p);
  }

  memset(buf, 0, 1024);

  ReceivePacket();
}

void Client::ReceivePacket()
{
  if (connected == true)
  {
    int res = 0;
    res = recv(sock, buf, 1024, 0);

    if (res > 0)
    {
      char *temp = buf;

      while (*temp != 0)
      {
        unsigned size = *(reinterpret_cast<unsigned *>(buf));

        packets.push_back(Packet(buf, size));

        PacketTypes t = packets.back().GetType();

        temp += size;
      }

      for (Packet & p : packets)
      {
        switch (p.GetType())
        {
          case PacketTypes::TEXT:
          {
            TraceLog::Log(TRACE_LEVEL::NETWORK, p.GetData());
            std::string s = p.GetItemPtr<char>();
            std::cout << s << std::endl;
            break;
          }
          case PacketTypes::C_CHECK:
          {
            TraceLog::Log(TRACE_LEVEL::NETWORK, "Received connectivity check packet.");
            break;
          }
          case PacketTypes::PING:
          {
            curr = clock.now();
            time = curr - prev;
            std::string msg = "Ping is ";
            msg += std::to_string(static_cast<int>(time.count()));
            msg += ".";
            TraceLog::Log(TRACE_LEVEL::INFO, msg);

            ping = false;
            break;
          }
          case PacketTypes::LOGIN:
          {
            std::string s = p.GetItemPtr<char>();

            glm::vec2 pos = p.GetItem<glm::vec2>();

            unsigned i = p.GetItem<unsigned>();

            GameObjectFactory::CreatePlayer(false, pos, i);

            break;
          }
          case PacketTypes::P_DISC:
          {
            std::string s = p.GetItemPtr<char>();
            TraceLog::Log(TRACE_LEVEL::NETWORK, s);

            unsigned i = p.GetItem<unsigned>();
            GameObjectFactory::DeletePlayer(i);
            break;
          }
          case PacketTypes::INIT:
          {
            glm::vec2 pos = p.GetItem<glm::vec2>();
            unsigned i = p.GetItem<int>();

            GameObjectFactory::CreatePlayer(true, pos, i);

            int s = p.GetItem<int>();

            for (int i = 0; i < s; ++i)
            {
              glm::vec2 position = p.GetItem<glm::vec2>();
              unsigned u = p.GetItem<unsigned>();
              GameObjectFactory::CreatePlayer(false, position, u);
            }

            break;
          }
          case PacketTypes::MOVE:
          {
            unsigned u = p.GetItem<unsigned>();

            for (unsigned i = 0; i < u; ++i)
            {
              unsigned id = p.GetItem<unsigned>();
              GameObject *player = GameObjectFactory::GetPlayer(id);

              glm::vec2 pos = p.GetItem<glm::vec2>();

              if (player)
              {
                PlayerComponent *c = GetComponent(player, PlayerComponent);
                c->SetNextPosition(pos);
                //TransformComponent *trans = GetComponent(player, TransformComponent);
                //trans->SetTranslation(pos);
              }
            }

            break;
          }
          case PacketTypes::CH_CHECK:
          {
            int channel = p.GetData<int>();
            std::cout << "Enter which channel you would like to switch to. (You are currently on channel " << channel << ")" << std::endl;
            int newch = 0;
            std::cin >> newch;
            Packet p(PacketTypes::CH_CHANGE, newch);
            SendPacket(p);
            chChange = false;
            break;
          }
          case PacketTypes::SSHUT:
          {
            TraceLog::Log(TRACE_LEVEL::ERR, "Connection with server lost!  Closing game.");
            Engine::CloseWindow();
            break;
          }
          default:
          {
            TraceLog::Log(TRACE_LEVEL::WARN, "Packet type not recognized!");
            break;
          }
        }
      }

      packets.clear();
    }
  }
}

void Client::SendPacket(Packet &p)
{
  int res = send(sock, p, p.GetSize(), 0);

  if (res == SOCKET_ERROR)
  {
    std::string msg = "Failed to send message '";
    msg += p.GetDesc();
    msg += "'.  Error code ";
    msg += std::to_string(WSAGetLastError());
    msg += ".  Closing game.";
    TraceLog::Log(TRACE_LEVEL::FATAL, msg);
    Engine::CloseWindow();
  }
  else
  {
    std::string msg = "Sent message '";
    msg += p.GetDesc();
    msg += "'.";
    TraceLog::Log(TRACE_LEVEL::NETWORK, msg);
  }
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