#pragma once

#include <glad.h>

#include "Shader.h"
#include "Texture.h"

class Square
{
public:
  Square();
  Square(const std::string &);
  Square(const std::string &, const std::string &);
  ~Square();

  virtual void Draw();

  Shader &GetShader()
  {
    return shader;
  }

  Texture &GetTexture()
  {
    return texture;
  }

protected:
  unsigned int EBO; //element buffer object, for more than 3 sided shapes
  unsigned int VAO; //vertex array object object
  unsigned int VBO; //vertex buffer object

  float vertices[36] =
  {  //position            //color?         //texture coords 
    0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 1.0f, 1.f, 1.0f, 1.0f,                            //top right of square
    0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f, 1.f, 1.0f, 0.0f,                            //bottom right
    -0.5f, -0.5f, 1.0f,  1.0f, 1.0f, 1.0f, 1.f, 0.0f, 0.0f,                            //bottum left
    -0.5f,  0.5f, 1.0f,  1.0f, 1.0f, 1.0f, 1.f, 0.0f, 1.0f                            //top left
  };

private:
  unsigned int indices[6] =
  {
    0, 1, 3, //the first triangle
    1, 2, 3  //the second triangle
  };

  Shader shader;
  Texture texture;
};

class FrameBufferSquare : public Square
{
public:
  FrameBufferSquare(GLuint text) : Square("FrameBuffer")
  {
      //  janky way of setting this to full screen size
    vertices[0] = 1.f;
    vertices[1] = 1.f;
    vertices[9] = 1.f;
    vertices[10] = -1.f;
    vertices[18] = -1.f;
    vertices[19] = -1.f;
    vertices[27] = -1.f;
    vertices[28] = 1.f;
    Square::GetTexture().SetID(text);

      //  setting them in vram
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //reinitiate the buffer data, with the points, modified
  }

  void Draw() override;

private:

};