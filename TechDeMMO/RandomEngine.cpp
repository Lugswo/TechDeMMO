#include "RandomEngine.h"

#include <stdlib.h>
#include <time.h>

RandomEngine::~RandomEngine()
{

}

void RandomEngine::Init()
{
  srand(time(NULL));
}

int RandomEngine::Int(int min, int max)
{
  if (min == max || (min == max))
  {
    return min;
  }
  // Get a random int
  int random = rand();

  // Constrain the value to a certain range
  random %= (max - min);
  return random + min;
}

float RandomEngine::Float(float min, float max)
{
  // Get a random float
  float random = float(rand());

  // Constrain the number to a certain range and return
  random = (RAND_MAX - random) / RAND_MAX;
  return random * (max - min) + min;
}

unsigned RandomEngine::Unsigned(unsigned min, unsigned max)
{
  if (min == max || (min == max))
  {
    return min;
  }
  // Get a random int
  unsigned random = rand();

  // Constrain the value to a certain range
  random %= (max - min);
  return random + min;
}
