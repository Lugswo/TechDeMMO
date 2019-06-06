#include <iostream>

#include "Server.h"

int main()
{
  Server::Init();
  Server::Update();
  Server::Shutdown();

  return 0;
}