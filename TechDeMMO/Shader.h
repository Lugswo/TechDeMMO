#pragma once

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
  Shader(const std::string & filename);
  ~Shader();

  const unsigned GetShader() const;
  void Use() const;
  //Shader operator=(Shader &rhs);

    //  Uniforms
  void Uniform(const std::string &, const int);
  void Uniform(const std::string & name, const glm::mat4 &mat);

  static void UniformToAllShaders(const std::string &, const glm::mat4 &);

private:
  Shader();
  Shader(const std::string &filename, int);
  unsigned id;
  std::string filename;
  bool loaded = true;

  unsigned CompileVertex(const std::ifstream & file);
  unsigned CompileFragment(const std::ifstream & file);

  static std::map<std::string, unsigned> shaders;
};