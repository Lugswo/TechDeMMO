#pragma once

class Component
{
public:
  Component();
  ~Component();

  virtual void Update(float dt) = 0;
};