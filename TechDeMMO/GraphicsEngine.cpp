#include "GraphicsEngine.h"

#include <glad.h>

#include "WindowManagement.h"

#include "TraceLog.h"

GLFWwindow *GraphicsEngine::window;

void GraphicsEngine::Init(int w, int h)
{
  window = CreateGLFWWindow(w, h);

  //glGenFramebuffers(1, &fbo);
  //glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  //// create a color attachment texture

  //glGenTextures(2, colorBuffers);

  //for (int i = 0; i < 2; ++i)
  //{
  //  glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
  //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
  //}
  //// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
  //unsigned int rbo;
  //glGenRenderbuffers(1, &rbo);
  //glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
  //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
  //// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
  //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  //  std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

  //unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
  //glDrawBuffers(2, attachments);

  //glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //square = new FrameBufferSquare(colorBuffers[0]);
  //Shader* s = square->GetShader();
  //s->Use();
  //s->IntUniform("screenTexture", 0);
  //s->IntUniform("bloom", 1);

  //glDeleteRenderbuffers(1, &rbo);

  //glGenFramebuffers(2, pingpongFBO);
  //glGenTextures(2, pingpongBuffer);
  //for (unsigned int i = 0; i < 2; i++)
  //{
  //  glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
  //  glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
  //  glTexImage2D(
  //    GL_TEXTURE_2D, 0, GL_RGB16F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, NULL
  //  );
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //  glFramebufferTexture2D(
  //    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
  //  );
  //}
}

GraphicsEngine::~GraphicsEngine()
{

}


void GraphicsEngine::Update(float dt)
{
  glClearColor(.2f, .2f, .19607f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsEngine::Restart()
{
  //glDeleteFramebuffers(1, &fbo);
  //glDeleteTextures(2, colorBuffers);
  //delete square;

  //GenerateFramebuffer();
}
