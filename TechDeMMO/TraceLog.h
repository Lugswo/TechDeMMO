#pragma once

#include <string>
#include <iostream>
#include <fstream>

enum class TRACE_LEVEL
{
  NONE,
  ERR,     //  error code from something fatal
  FATAL,     //  something that crashes the program
  IMPORTANT, //  extremely important info like game start, game connection to server, etc
  WARN,      //  warnings that can lead to potential crashes
  INFO,      //  info for an average user (monster spawns, player moving areas, etc)
  DEBUG,     //  info for developers (testing certain data, making sure values are correct, etc)
  EDITOR,    //  info from the editor for debugging
  VERBOSE    //  everything
};

class TraceLog
{
public:
  static void Init();
  static void Log(TRACE_LEVEL verbosity, const std::string& message);

private:
  static TRACE_LEVEL level;
  static std::ofstream file;
  static bool consoleOut;

  static bool generateTrace;
};