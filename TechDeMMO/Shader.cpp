#include "Shader.h"

#include <fstream>
#include <sstream>
#include <glad.h>

#include "TraceLog.h"

std::map<std::string, unsigned> Shader::shaders;

Shader::Shader(const std::string & filename)
{
  id = 0;
  this->filename = filename;
  std::string s = "Constructing shader with filename ";
  s += filename;
  s += ".";
  TraceLog::Log(TRACE_LEVEL::INFO, s);

  if (!shaders.empty())
  {
    auto sh = shaders.find(filename);

    if (sh != shaders.end())
    {
      id = (*sh).second;
      TraceLog::Log(TRACE_LEVEL::INFO, "Precreated shader " + filename + " found.");
    }
  }

  if (!id)
  {
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
      TraceLog::Log(TRACE_LEVEL::VERBOSE, "Found vertex shader " + vs + ".");
    }

    std::string fs = filename;
    fs += ".fs";
    std::ifstream f(fs);

    if (f.bad())
    {
      TraceLog::Log(TRACE_LEVEL::ERR, "Could not find fragment shader " + fs + "!");
      loaded = false;
    }
    else
    {
      TraceLog::Log(TRACE_LEVEL::VERBOSE, "Found fragment shader " + fs + ".");
    }

    unsigned vID = CompileVertex(v);
    unsigned fID = CompileFragment(f);

    id = glCreateProgram();

    //link compiled shaders to shader program
    glAttachShader(id, vID);
    glAttachShader(id, fID);
    glLinkProgram(id);

    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(id, 512, NULL, infoLog);

      std::string msg = "Linking or creating the shader program has failed.  ";
      msg += infoLog;
      TraceLog::Log(TRACE_LEVEL::ERR, msg);
    }

    TraceLog::Log(TRACE_LEVEL::INFO, filename + " shader successfully created.");

    // delete the created shaders
    glDeleteShader(vID);
    glDeleteShader(fID);

    shaders[filename] = id;
  }
}

Shader::~Shader()
{

}

void Shader::Use() const
{
  glUseProgram(id);
}

unsigned Shader::CompileVertex(const std::ifstream & file)
{
  std::string vertexShaderSource; //the source code for the vertex shader

  std::stringstream vertexShaderContents; //contents of the shader

  vertexShaderContents << file.rdbuf(); //read in file to string stream

  vertexShaderSource = vertexShaderContents.str(); //move contents to a string
  const char* vertexCopy = vertexShaderSource.c_str(); //convert string to const char*;


  unsigned int vertexShader; //make vertexshader?
  vertexShader = glCreateShader(GL_VERTEX_SHADER); //creates a vertex shader

  glShaderSource(vertexShader, 1, &vertexCopy, NULL); //should compile the vertex shader
  glCompileShader(vertexShader);

  //check if shader compiled
  int success; //more or less a bool
  char infoLog[512];  //replace with westons logs
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //preforms a check

  if (!success)
  {
    std::string msg = "Compiling vertex shader failed.  ";
    msg += infoLog;
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//gets the log
    TraceLog::Log(TRACE_LEVEL::ERR, msg);
  }

  TraceLog::Log(TRACE_LEVEL::VERBOSE, "Vertex shader for " + filename + " shader compiled successfully.");

  return vertexShader;
}

unsigned Shader::CompileFragment(const std::ifstream & file)
{
  std::string vertexShaderSource; //the source code for the vertex shader

  std::stringstream vertexShaderContents; //contents of the shader

  vertexShaderContents << file.rdbuf(); //read in file to string stream

  vertexShaderSource = vertexShaderContents.str(); //move contents to a string
  const char* vertexCopy = vertexShaderSource.c_str(); //convert string to const char*;


  unsigned int vertexShader; //make vertexshader?
  vertexShader = glCreateShader(GL_FRAGMENT_SHADER); //creates a vertex shader

  glShaderSource(vertexShader, 1, &vertexCopy, NULL); //should compile the vertex shader
  glCompileShader(vertexShader);

  //check if shader compiled
  int success; //more or less a bool
  char infoLog[512];  //replace with westons logs
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //preforms a check

  if (!success)
  {
    std::string msg = "Compiling fragment shader failed.  ";
    msg += infoLog;
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);//gets the log
    TraceLog::Log(TRACE_LEVEL::ERR, msg);
  }

  TraceLog::Log(TRACE_LEVEL::VERBOSE, "Fragment shader for " + filename + " shader compiled successfully.");

  return vertexShader;
}

void Shader::UniformToAllShaders(const std::string & name, const glm::mat4 & mat)
{
  for (auto itr = shaders.begin(); itr != shaders.end(); ++itr)
  {
    glUseProgram((*itr).second);
    GLint loc = glGetUniformLocation((*itr).second, name.c_str());
    if (loc != -1)
      glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
  }
}

void Shader::Uniform(const std::string & name, int i)
{
  Use();
  GLint loc = glGetUniformLocation(id, name.c_str());
  glUniform1i(loc, i);
}

void Shader::Uniform(const std::string & name, const glm::mat4 & mat)
{
  Use();
  GLint loc = glGetUniformLocation(id, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}