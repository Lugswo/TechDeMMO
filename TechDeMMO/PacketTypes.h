#pragma once

enum class PacketTypes
{
    //  initialization
  LOGIN,
  INIT,
  C_CHECK,
  VERSION,

    //  game packets
  TEXT,
  JOIN,
  PING,
  CH_CHANGE,
  P_DISC,

    //  player input
  MOVE,

    //  utility packets
  CH_CHECK,

    //  shutdown
  CSHUT,
  SSHUT
};
