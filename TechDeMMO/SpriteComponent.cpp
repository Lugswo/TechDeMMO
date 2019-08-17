#include "SpriteComponent.h"

#include "GraphicsEngine.h"

SpriteComponent::SpriteComponent(const std::string & filename) : square(GraphicsEngine::CreateSprite(filename, id))
{
  this->filename = filename;
}

SpriteComponent::~SpriteComponent()
{
  GraphicsEngine::RemoveSprite(id);
}

void SpriteComponent::Init()
{
  trans = GetComponent(parent, TransformComponent);
}

void SpriteComponent::Update(float dt)
{
  glm::mat4 p = trans->GetMatrix();
  square.SetTransform(p);
}