#include "TraceLog.h"

#include <chrono>
#include <ctime>
#include <algorithm>

TRACE_LEVEL TraceLog::level = TRACE_LEVEL::VERBOSE;
std::ofstream TraceLog::file;
bool TraceLog::consoleOut = true;
bool TraceLog::generateTrace = true;

static std::string SetupString()
{
  std::string str;
  time_t theTime = time(NULL);
  struct tm *aTime = localtime(&theTime);

  int hour = aTime->tm_hour;
  int min = aTime->tm_min;
  int sec = aTime->tm_sec;

  str = "[";
  if (hour < 10)
    str += "0";
  str += std::to_string(hour);
  str += ":";
  if (min < 10)
    str += "0";
  str += std::to_string(min);
  str += ":";

  if (sec < 10)
    str += "0";
  str += std::to_string(sec);
  str += "] [";

  return str;
}

void TraceLog::Init()
{
  if (generateTrace)
  {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::string filename = "Logs/Trace ";
    filename += std::ctime(&end_time);
    filename.back() = '.';
    filename += "log";

    std::replace(filename.begin(), filename.end(), ':', '.');

    file.open(filename);

    if (file.bad())
    {
      std::cout << "ERROR: File could not be opened" << std::endl;
    }
    else
    {
      file << "=============================================" << std::endl;

      std::string str = std::ctime(&end_time);
      str.back() = ' ';

      file << std::endl << "Log for " << str << "started." << std::endl << std::endl;
      file << "=============================================" << std::endl << std::endl;
    }
  }
}

void TraceLog::Log(TRACE_LEVEL verbosity, const std::string& message)
{
  if (generateTrace)
  {
    if (verbosity <= level)
    {
      std::string str = SetupString();

      switch (verbosity)
      {
        case TRACE_LEVEL::DEBUG:
          str += "DEBUG";
          break;
        case TRACE_LEVEL::IMPORTANT:
          str += "IMPRT";
          break;
        case TRACE_LEVEL::FATAL:
          str += "FATAL";
          break;
        case TRACE_LEVEL::ERR:
          str += "ERROR";
          break;
        case TRACE_LEVEL::INFO:
          str += "INFOM";
          break;
        case TRACE_LEVEL::VERBOSE:
          str += "VBOSE";
          break;
        case TRACE_LEVEL::WARN:
          str += "WARNG";
          break;
        case TRACE_LEVEL::EDITOR:
          str += "EDITR";
          break;
        default:
          str += "BROKE";
          break;
      }

      str += "] ";
      str += message;

      if (consoleOut)
        std::cout << str << std::endl;

      file << str << std::endl;
    }
  }
}