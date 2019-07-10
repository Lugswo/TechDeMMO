#include "Shader.h"

#include <fstream>

#include "TraceLog.h"

Shader::Shader(const std::string & filename)
{
  TraceLog::Log(TRACE_LEVEL::INFO, "Constructing shader with filename " + filename);

  std::string vs = filename;
  vs += ".vs";
  std::ifstream v(vs);

  if (v.bad())
  {
    TraceLog::Log(TRACE_LEVEL::ERR, "Could not find vertex shader " + vs + "!");
    loaded = false;
  }
  else
  {
    TraceLog::Log(TRACE_LEVEL::INFO, "Found vertex shader " + vs + ".");
  }

  std::string fs = filename;
  fs += ".fs";
  std::ifstream f(fs);

  if (f.bad())
  {
    TraceLog::Log(TRACE_LEVEL::ERR, "Could not find fragment shader " + vs + "!");
    loaded = false;
  }
  else
  {
    TraceLog::Log(TRACE_LEVEL::INFO, "Found fragment shader " + vs + ".");
  }
}

Shader::Shader(unsigned int i)
{
  id = i;
}

Shader Shader::operator=(Shader &rhs)
{
  return Shader(rhs.id);
}