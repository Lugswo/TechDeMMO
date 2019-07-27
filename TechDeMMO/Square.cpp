#include "Square.h"

Square::Square() : shader("Texture"), texture("ArtAssets/EngineTextures/MissingTexture.png")
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO); //generate the VBO object, holds 1 triangles worth of vertices
  glGenBuffers(1, &EBO); //make the element buffer, that will uses the limited vertices

  glBindVertexArray(VAO); //bind a vertex array object

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //initiate the buffer data, with the points

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the element buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //set the indices
  //load vertex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  //load color
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //load texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

Square::Square(const std::string & f1) : shader(f1), texture("ArtAssets/EngineTextures/MissingTexture.png")
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO); //generate the VBO object, holds 1 triangles worth of vertices
  glGenBuffers(1, &EBO); //make the element buffer, that will uses the limited vertices

  glBindVertexArray(VAO); //bind a vertex array object

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //initiate the buffer data, with the points

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the element buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //set the indices
  //load vertex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  //load color
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //load texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

Square::Square(const std::string &f1, const std::string &f2) : shader(f1), texture(f2)
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO); //generate the VBO object, holds 1 triangles worth of vertices
  glGenBuffers(1, &EBO); //make the element buffer, that will uses the limited vertices

  glBindVertexArray(VAO); //bind a vertex array object

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //initiate the buffer data, with the points

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //bind the element buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //set the indices
  //load vertex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  //load color
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //load texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

Square::~Square()
{

}

void Square::Draw()
{
  //glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //reinitiate the buffer data, with the points, modified

  texture.Use();
  shader.Use();

  glBindVertexArray(VAO); //refresh the VAO
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw the triangles

  glUseProgram(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0); //refresh the VAO
}

void FrameBufferSquare::Draw()
{
  Square::GetShader().Use();
  glBindVertexArray(VAO);
  
  Square::GetTexture().Use();
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw the triangles
}