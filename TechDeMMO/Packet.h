#pragma once

#include <string>
#include "PacketTypes.h"

class Packet
{
public:
  Packet(char *data);

  template <typename T>
  Packet(PacketTypes p, const T &d, const std::string&s = "")
  {
    beginData = new char[sizeof(PacketTypes) + sizeof(T)];
    size = sizeof(PacketTypes) + sizeof(T);
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData);
    *pa = p;
    T *temp = reinterpret_cast<T *>(beginData + sizeof(PacketTypes));
    *temp = d;

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  Packet(PacketTypes p, const std::string &d, const std::string&s = "")
  {
    size = sizeof(PacketTypes) + d.length();
    beginData = new char[size];
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData);
    *pa = p;
    char * temp = reinterpret_cast<char *>(beginData + sizeof(PacketTypes));
    memcpy(temp, d.c_str(), d.length());

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  template <typename T>
  Packet(PacketTypes p, T *d, int num = -1, const std::string& s = "")
  {
    if (num = -1)
      num = sizeof(T);

    beginData = new char[sizeof(PacketTypes) + num];
    size = sizeof(PacketTypes) + num;
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData);
    *pa = p;
    T *temp = beginData + sizeof(PacketTypes);
    void *t2 = beginData + sizeof(PacketTypes);
    memcpy(t2, d, num);

    data = (char*)(temp);

    desc = s;
  }

  ~Packet();

  const char * GetData() const;
  const int GetSize() const
  {
    return size;
  }

  operator char*();

  const std::string& GetDesc() const
  {
    if (desc == "")
      return "No description given.";
    return desc;
  }

private:
  PacketTypes type;
  char *data;

    //  used for deleting all data
  char *beginData;

  std::string desc;

  int size = 0;
};