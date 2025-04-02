#include <iostream>
#include "Blur.h"
#include "Particle.h"

void main()
{
  int index = 0;
  std::cin >> index;

  switch (index)
  {
    case 0:
      CircleExample();
      break;
    case 1:
      ParticleExample();
      break;
  }

  return;
}