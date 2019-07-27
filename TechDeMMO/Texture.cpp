#include "Texture.h"

#include <fstream>
#include <glad.h>
#include "stb_image.h"

#include "TraceLog.h"

std::map<std::string, unsigned> Texture::textures;

Texture::Texture(const std::string &filename)
{
  id = 0;
  this->filename = filename;
  TraceLog::Log(TRACE_LEVEL::INFO, "Constructing texture with filename " + filename);

  auto tex = textures.find(filename);

  if (tex != textures.end())
  {
    id = (*tex).second;
    TraceLog::Log(TRACE_LEVEL::INFO, "Found precreated texture " + filename + ".");
  }

  if (!id)
  {
    std::ifstream file(filename);

    if (file.bad())
    {
      TraceLog::Log(TRACE_LEVEL::ERR, "Could not find texture " + filename + "!");
      loaded = false;
    }
    else
    {
      TraceLog::Log(TRACE_LEVEL::VERBOSE, "Found texture " + filename + ".");
    }

    glGenTextures(1, &id); // generate a texture
    glBindTexture(GL_TEXTURE_2D, id);

    //set texture wrapping and filters std while working on tutorial
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    std::string fname;
    int size = (int)filename.size();
    for (int i = 0; i < size; i++)
    {
      if (filename[i] == '/')
        fname += '\\';
      else
        fname += filename[i];
    }

    int width, height, nrChannels;

    unsigned char *data = stbi_load(fname.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      TraceLog::Log(TRACE_LEVEL::INFO, "Loaded texture " + filename + " correctly.");
    }
    else
    {
      TraceLog::Log(TRACE_LEVEL::ERR, "Failed to load texture " + filename + "!  Using missing texture instead.");
    }
    stbi_image_free(data);

    textures[filename] = id;
  }
}

Texture::Texture(const unsigned id)
{
  this->id = id;
}

Texture::~Texture()
{

}

void Texture::Use() const
{
  glBindTexture(GL_TEXTURE_2D, id);
}