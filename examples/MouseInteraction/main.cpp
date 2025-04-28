#include "AABB_RayCast.h"
#include "MullerTrumbore_RayCast.h"

int main()
{
  int index = 0;
  switch (index)
  {
    case 0:
      AABB_RayCast();
      break;
    case 1:
      MullerTrumbore_RayCast();
      break;
  }
  return 0;
}