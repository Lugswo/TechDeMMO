#include "Packet.h"

#include <cstring>

Packet::Packet(char *d, unsigned u)
{
  memcpy(beginData, d, u);

  PacketTypes *p = reinterpret_cast<PacketTypes *>(d + sizeof(unsigned));

  type = *p;
  data = d + sizeof(PacketTypes) + sizeof(unsigned);
}

const char * Packet::GetData() const
{
  return data;
}

bool Packet::CheckAfterData()
{
  if (data + offset != nullptr)
  {
    return true;
  }
  return false;
}

Packet::operator char *()
{
  return beginData;
}

Packet::~Packet()
{

}
