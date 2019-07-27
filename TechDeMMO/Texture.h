#pragma once

#include <map>

class Texture
{
public:
  Texture(const std::string& filename);
  Texture(const unsigned);
  ~Texture();

    //  Never call unless using framebuffer stuff
  void SetID(const unsigned i)
  {
    id = i;
  }

  const unsigned GetID() const
  {
    return id;
  }

  void Use() const;

private:
  Texture();
  
  unsigned id;
  std::string filename;
  bool loaded = true;

  static std::map<std::string, unsigned> textures;
};