#pragma once

#include <string>
#include <vector>

#include "PacketTypes.h"

const unsigned MAX = 1024;

class Packet
{
public:
  Packet(char *data, unsigned u);

  template <typename T>
  Packet(PacketTypes p, const T &d, const std::string&s = "")
  {
    size = sizeof(PacketTypes) + sizeof(T) + sizeof(unsigned);
    unsigned *i = reinterpret_cast<unsigned *>(beginData);
    *i = size;
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData + sizeof(unsigned));
    *pa = p;
    T *temp = reinterpret_cast<T *>(beginData + sizeof(PacketTypes) + sizeof(unsigned));
    *temp = d;

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  Packet(PacketTypes p, const std::string &d, const std::string&s = "")
  {
    size = static_cast<int>(sizeof(PacketTypes) + d.length() + 1 + sizeof(unsigned));
    unsigned *i = reinterpret_cast<unsigned *>(beginData);
    *i = size;
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData + sizeof(unsigned));
    *pa = p;

    char * temp = reinterpret_cast<char *>(beginData + sizeof(PacketTypes) + sizeof(unsigned));
    memcpy(temp, d.c_str(), d.length() + 1);

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  template <typename T>
  Packet(PacketTypes p, const std::vector<T> v, const std::string &s = "")
  {
    size = static_cast<int>(sizeof(PacketTypes) + (v.size() * sizeof(T)) + (sizeof(unsigned) * 2));

    unsigned *i = reinterpret_cast<unsigned *>(beginData);
    *i = size;
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData + sizeof(unsigned));
    *pa = p;

    unsigned * temp = reinterpret_cast<unsigned *>(beginData + sizeof(PacketTypes) + sizeof(unsigned));
    *temp = static_cast<unsigned>(v.size());

    void * t2 = reinterpret_cast<void *>(beginData + sizeof(PacketTypes) + (sizeof(unsigned) * 2));
    memcpy(t2, v.data(), (v.size() * sizeof(T)));

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  Packet(PacketTypes p, const char *d, const std::string&s = "")
  {
    size = static_cast<int>(sizeof(PacketTypes) + strlen(d) + 1 + sizeof(unsigned));

    unsigned *i = reinterpret_cast<unsigned *>(beginData);
    *i = size;
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData + sizeof(unsigned));
    *pa = p;

    char * temp = reinterpret_cast<char *>(beginData + sizeof(PacketTypes) + sizeof(unsigned));
    memcpy(temp, d, strlen(d) + 1);

    data = reinterpret_cast<char *>(temp);

    desc = s;
  }

  Packet(PacketTypes p)
  {
    size = static_cast<int>(sizeof(PacketTypes) + 1 + sizeof(unsigned));

    unsigned *i = reinterpret_cast<unsigned *>(beginData);
    *i = size;
    PacketTypes *pa = reinterpret_cast<PacketTypes *>(beginData + sizeof(unsigned));
    *pa = p;

    const char * g = "e";
    char * temp = reinterpret_cast<char *>(beginData + sizeof(PacketTypes) + sizeof(unsigned));
    memcpy(temp, g, 1);
    desc = "";

    data = reinterpret_cast<char *>(temp);
  }

  ~Packet();

  template <typename T>
  void AddItem(T d)
  {
    //char *newDat = new char[size + sizeof(T)];
    //memset(newDat, 0, size + sizeof(T));
    //memcpy(newDat, beginData, size);

    //char *temp = beginData;
    //beginData = newDat;
    //data = newDat + sizeof(PacketTypes);

    T *t = reinterpret_cast<T*>(beginData + size);
    *t = d;

    size += sizeof(T);

    unsigned *i = reinterpret_cast<unsigned *>(beginData);
    *i = size;
    //delete[] temp;
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
  const unsigned GetSize() const
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
  T& GetData()
  {
    T *ptr = reinterpret_cast<T *>(data);
    offset += sizeof(T);
    return *ptr;
  }

  bool CheckAfterData();

private:
  PacketTypes type;
  char *data;

    //  used for deleting all data
  char beginData[MAX];

    //  not sent with the packet so safe to store as many descriptors as u want
  std::string desc;

  const std::string defDesc = "No description given.";

  unsigned size = 0;

  unsigned offset = 0;
};