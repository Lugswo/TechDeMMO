#include "Packet.h"

#include <cstring>

Packet::Packet(char *d)
{
  beginData = d;

  PacketTypes *p = reinterpret_cast<PacketTypes *>(d);

  type = *p;
  data = d + sizeof(PacketTypes);
}

const char * Packet::GetData() const
{
  return data;
}

Packet::operator char *()
{
  return beginData;
}

Packet::~Packet()
{
  delete beginData;
}
