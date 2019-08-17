#pragma once

class RandomEngine
{
public:
  ~RandomEngine();
  static void Init();
  static int Int(int min, int max);
  static float Float(float min, float max);
  static unsigned Unsigned(unsigned min, unsigned max);

private:
  RandomEngine();
};