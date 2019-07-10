#pragma once

#include <vector>
#include <string>

class Shader
{
public:
  Shader(const std::string & filename);
  ~Shader();

  const unsigned GetShader() const;
  void Use() const;
  Shader operator=(Shader &rhs);

private:
  Shader();
  Shader(unsigned int);
  unsigned id;
  std::string vertex, frag;
  bool loaded = true;

  static std::vector<Shader> shaders;
};