#pragma once

#include <string>
#include <vector>

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
    size = static_cast<int>(sizeof(PacketTypes) + d.length() + 1);
    beginData = new char[size];
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData);
    *pa = p;
    char * temp = reinterpret_cast<char *>(beginData + sizeof(PacketTypes));
    memcpy(temp, d.c_str(), d.length() + 1);

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  template <typename T>
  Packet(PacketTypes p, const std::vector<T> v, const std::string &s = "")
  {
    size = static_cast<int>(sizeof(PacketTypes) + (v.size() * sizeof(T)) + sizeof(unsigned));
    beginData = new char[size];
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData);
    *pa = p;

    unsigned * temp = reinterpret_cast<unsigned *>(beginData + sizeof(PacketTypes));
    *temp = static_cast<unsigned>(v.size());

    void * t2 = reinterpret_cast<void *>(beginData + sizeof(PacketTypes) + sizeof(unsigned));
    memcpy(t2, v.data(), (v.size() * sizeof(T)));

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  Packet(PacketTypes p, const char *d, const std::string&s = "")
  {
    size = static_cast<int>(sizeof(PacketTypes) + strlen(d) + 1);
    beginData = new char[size];
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData);
    *pa = p;
    char * temp = reinterpret_cast<char *>(beginData + sizeof(PacketTypes));
    memcpy(temp, d, strlen(d) + 1);

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

  Packet(PacketTypes p)
  {
    size = static_cast<int>(sizeof(PacketTypes) + 1);
    beginData = new char[size];
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData);
    *pa = p;
    const char * g = "e";
    char * temp = reinterpret_cast<char *>(beginData + sizeof(PacketTypes));
    memcpy(temp, g, 1);
    desc = "";

    data = reinterpret_cast<char *>(temp);
  }

  ~Packet();

  template <typename T>
  void AddItem(T d)
  {
    char *newDat = new char[size + sizeof(T)];
    memset(newDat, 0, size + sizeof(T));
    memcpy(newDat, beginData, size);

    char *temp = beginData;
    beginData = newDat;
    data = newDat + sizeof(PacketTypes);

    T *t = reinterpret_cast<T*>(beginData + size);
    *t = d;

    size += sizeof(T);
    delete[] temp;
  }

  template <typename T>
  const T &GetItem()
  {
    T * temp = reinterpret_cast<T *>(data + offset);
    offset += sizeof(T);

    return *temp;
  }

  template <typename T>
  const T *GetItemPtr()
  {
    T * temp = reinterpret_cast<T *>(data + offset);
    offset += sizeof(T);

    return temp;
  }

  template <>
  const char * GetItemPtr<char>()
  {
    char * temp = data + offset;
    int t = strlen(temp);

    offset += t;
    offset += 1;
    return temp;
  }

  const char * GetData() const;
  const int GetSize() const
  {
    return size;
  }

  const PacketTypes GetType() const
  {
    return type;
  }

  operator char*();

  const std::string& GetDesc() const
  {
    if (desc == "")
      return defDesc;
    return desc;
  }

  template <typename T>
  T& GetData() const
  {
    T *ptr = reinterpret_cast<T *>(data);
    return *ptr;
  }

private:
  PacketTypes type;
  char *data;

    //  used for deleting all data
  char *beginData;

    //  not sent with the packet so safe to store as many descriptors as u want
  std::string desc;

  const std::string defDesc = "No description given.";

  int size = 0;

  int offset = 0;
};