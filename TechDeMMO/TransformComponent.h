#pragma once

#include "Component.h"

#include <glm/glm.hpp>

class TransformComponent : public Component
{
public:
  TransformComponent();
  ~TransformComponent();

  void Update(float dt)
  {

  }

  const glm::mat4 & GetMatrix();

  void SetTranslation(const glm::vec2 &);
  void SetTranslation(const glm::vec3 &);
  void SetScale(const glm::vec2 &);
  void SetRotation(const float);

  const glm::vec3 & GetTranslation() const
  {
    return t;
  }

  const glm::vec2 & GetScale() const
  {
    return s;
  }

  const float GetRotation() const
  {
    return r;
  }

  NAME(TransformComponent)

private:
  glm::mat4 transform, sm, rm, tm;
  glm::vec3 t;
  glm::vec2 s;

  float r;

  bool sd, rd, td, dirty;
};