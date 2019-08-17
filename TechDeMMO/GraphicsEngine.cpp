#include "GraphicsEngine.h"

#include <glad.h>

#include "WindowManagement.h"
#include "Shader.h"
#include "Square.h"

#include "TraceLog.h"

GLFWwindow *GraphicsEngine::window;

GLuint GraphicsEngine::fbo, GraphicsEngine::colorBuffer;
FrameBufferSquare *GraphicsEngine::square;

int GraphicsEngine::sWidth, GraphicsEngine::sHeight;
unsigned GraphicsEngine::id;

bool GraphicsEngine::loaded = true;

glm::mat4 GraphicsEngine::proj;

Square *GraphicsEngine::guy;

std::map<int, Square> GraphicsEngine::sprites;

void GraphicsEngine::Init(int w, int h)
{
  id = 0;
  window = CreateGLFWWindow(w, h);

  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  WindowResize(window, w, h);

  glfwSetWindowSizeCallback(window, WindowResize);
}

void GraphicsEngine::GenerateFrameBuffer()
{
  TraceLog::Log(TRACE_LEVEL::INFO, "Generating framebuffer.");
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  // create a color attachment texture

  glGenTextures(1, &colorBuffer);

  glBindTexture(GL_TEXTURE_2D, colorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, sWidth, sHeight, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

  // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sWidth, sHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    TraceLog::Log(TRACE_LEVEL::FATAL, "FrameBuffer not created!  Unable to start game.");
    loaded = false;
  }

  unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, attachments);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  square = new FrameBufferSquare(colorBuffer);
  Shader s = square->GetShader();
  s.Use();
  s.Uniform("screenTexture", 0);

  glDeleteRenderbuffers(1, &rbo);

  if (loaded)
    TraceLog::Log(TRACE_LEVEL::INFO, "FrameBuffer created successfully.");
}

GraphicsEngine::~GraphicsEngine()
{
  delete square;
}

void GraphicsEngine::WindowResize(GLFWwindow *window, int width, int height)
{
  proj = glm::mat4(1.0f);
  sWidth = width;
  sHeight = height;

  proj[0][0] = (1.f / (float)width) * 150.f;
  proj[1][1] = (1.f / (float)height) * 150.f;

  glViewport(0, 0, width, height);

  TraceLog::Log(TRACE_LEVEL::INFO, "Window resized to " + std::to_string(width) + "x" + std::to_string(height) + ".");
  Restart();
}

void GraphicsEngine::Update(float dt)
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glEnable(GL_DEPTH_TEST);

  glClearColor(.2f, .2f, .19607f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glActiveTexture(GL_TEXTURE0);

  Shader::UniformToAllShaders("projection", proj);

  for (auto itr = sprites.begin(); itr != sprites.end(); ++itr)
  {
    (*itr).second.SendTransform();
    (*itr).second.Draw();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST);

  glClearColor(0.f, 0.f, 0.f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  square->Draw();
}

void GraphicsEngine::Restart()
{
  glDeleteFramebuffers(1, &fbo);
  glDeleteTextures(1, &colorBuffer);
  delete square;

  GenerateFrameBuffer();
}

Square& GraphicsEngine::CreateSprite(const std::string & str, unsigned &i, const std::string & shader)
{
  ++id;
  i = id;
  sprites.insert(std::pair<int, Square>(id, Square(shader, str)));
  //sprites[id] = Square(shader, str);
  return sprites[id];
}

void GraphicsEngine::RemoveSprite(unsigned i)
{
  sprites.erase(i);
}