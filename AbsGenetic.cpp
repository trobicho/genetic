#include "AbsGenetic.h"

int AbsGenetic::wheel_sigma(void)//class Selector
{
    int r, i, min=1, max=m_nbPeople, n=randMinMax(1, sigma(m_nbPeople));
    if(n>sigma(m_nbPeople/2))
        min=m_nbPeople/2;
    else
        max=m_nbPeople/2;
    for(i=min; i<max; i++)
    {
        if(n>sigma(i))
            min=i;
        else
            max=i;
    }
    if(i==sigma(min))
        r=min;
    else
        r=max;
    return (m_nbPeople-r);
}

int AbsGenetic::sigma(int n)//class Selector ou Calcul
{
    return ((n+1)/2.0)*n;
}
