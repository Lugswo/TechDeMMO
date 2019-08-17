#pragma once

#include <string>

#include "GameObject.h"

#define NAME(n) const char * GetName() const { return #n; }

class Component
{
public:
  Component();
  virtual ~Component();

  virtual void Update(float dt) = 0;
  virtual void Init()
  {

  }

  void SetParent(GameObject * p)
  {
    parent = p;
  }

  virtual const char * GetName() const = 0;

protected:
  GameObject *parent;
};