#pragma once

enum class PacketTypes
{
    //  initialization
  LOGIN,
  C_CHECK,
  VERSION,

    //  actual packets
  TEXT,
  PING,
  CH_CHANGE,

    //  utility packets
  CH_CHECK,

    //  shutdown
  CSHUT,
  SSHUT
};