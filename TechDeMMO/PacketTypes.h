#pragma once

enum class PacketTypes
{
    //  initialization
  LOGIN,
  INIT,
  C_CHECK,
  VERSION,

    //  actual packets
  TEXT,
  JOIN,
  PING,
  CH_CHANGE,
  P_DISC,

    //  utility packets
  CH_CHECK,

    //  shutdown
  CSHUT,
  SSHUT
};