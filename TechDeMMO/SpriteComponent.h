#pragma once

#include "Component.h"

#include <string>

#include "Square.h"
#include "TransformComponent.h"

class SpriteComponent : public Component
{
public:
  SpriteComponent(const std::string &);
  ~SpriteComponent();

  void Init() override;
  void Update(float dt) override;

  const Square & GetSquare()
  {
    return square;
  }

  NAME(SpriteComponent)

private:
  SpriteComponent();

  unsigned id;
  TransformComponent *trans;
  std::string filename;
  Square & square;
};