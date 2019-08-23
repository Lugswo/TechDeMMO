#include "TransformComponent.h"

#include <glm\gtc\matrix_transform.hpp>

TransformComponent::TransformComponent()
{
  transform = sm = rm = tm = glm::mat4(1.f);
  sd = rd = td = dirty = true;

  t = glm::vec3(0.f);
  s = glm::vec2(1.f);
  r = 0.f;
}

TransformComponent::~TransformComponent()
{

}

const glm::mat4 & TransformComponent::GetMatrix()
{
  if (dirty)
  {
    if (td)
    {
      tm = glm::mat4(1.f);
      tm = glm::translate(tm, t);
      td = false;
    }
    if (sd)
    {
      sm = glm::mat4(1.f);
      sm = glm::scale(sm, glm::vec3(s.x, s.y, 1.f));
      sd = false;
    }
    if (rd)
    {
      rm = glm::mat4(1.f);
      rm = glm::rotate(rm, r, glm::vec3(0.f, 0.f, 1.f));
      rd = false;
    }

    transform = tm * rm * sm;

    dirty = false;
  }

  return transform;
}

void TransformComponent::SetRotation(const float rot)
{
  dirty = rd = true;
  r = rot;
}

void TransformComponent::SetScale(const glm::vec2 & scale)
{
  dirty = sd = true;
  s = scale;
}

void TransformComponent::SetTranslation(const glm::vec3 & translation)
{
  dirty = td = true;
  t = translation;
}

void TransformComponent::SetTranslation(const glm::vec2 & translation)
{
  dirty = td = true;
  t = glm::vec3(translation.x, translation.y, 0.f);
}