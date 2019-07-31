#include "random.h"

int randMinMax(int min, int max)
{
    return std::rand()%((max+1)-min)+min;
}
