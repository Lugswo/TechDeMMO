#pragma once

#include <glad.h>

class FrameBufferSquare
{
public:
  FrameBufferSquare(GLuint text);
  void draw();
  void UntexturedDraw();
  void TexturedDraw();

  Shader *GetShader()
  {
    return shader;
  }

private:
  //to consolidate space a square only needs 4 vertices to make 2 triangles
  float vertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
      // positions   // texCoords
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
       1.0f, -1.0f,  1.0f, 0.0f,

      -1.0f,  1.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  1.0f, 1.0f
  };  //however the draw function still needs to draw 2 triangles, with the indices as ofsets of the vertices
  unsigned int indices[6] =
  {
    0, 1, 3, //the first triangle
    1, 2, 3  //the second triangle
  };

  unsigned int EBO; //element buffer object, for more than 3 sided shapes
  unsigned int VAO; //vertex array object object
  unsigned int VBO; //vertex buffer object

  Shader *shader; //standard shader program
  GLuint texture;
};