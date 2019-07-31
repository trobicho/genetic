#include "PVC.h"
#include "../random.h"
#include <math.h>

#include <iostream>
void shuffle(int *tab, int l);

PVC::PVC(int nbCitys, s_vec2i min, s_vec2i max):m_nbCitys(nbCitys), m_min(min), m_max(max)
{
    m_city=new s_vec2i[nbCitys];
    for(int i=0; i<nbCitys; i++){m_city[i]=randCity();}
    m_regenTab=new int[nbCitys];
}

int PVC::evaluate(int *gene)
{
    int score=0;
    for(int i=0; i<m_nbCitys; i++)
    {
        score+=dist(m_city[gene[i]], m_city[gene[(i+1)%m_nbCitys]]);
    }
    return score;
}

void PVC::display(int *path)
{
}

void PVC::randGenome(int *gene)
{
    int i;
    for(i=0; i<m_nbCitys; i++)
    {
        gene[i]=i;
    }
    shuffle(gene, m_nbCitys);
}

void PVC::repairGenome(int *gene)
{
    int i, j, k;
    for(i=0; i<m_nbCitys; i++){m_regenTab[i]=0;}
    for(i=0; i<m_nbCitys; i++){m_regenTab[gene[i]]+=1;}
    for(i=0; i<m_nbCitys; i++)
    {
        if(m_regenTab[i]>1)
        {
            for(j=0; j<m_nbCitys && m_regenTab[j]!=0; j++);
            for(k=m_nbCitys-1; k>0 && gene[k]!=i; k--);
            gene[k]=j;
            m_regenTab[j]+=1;
            m_regenTab[i]-=1;
            i--;
        }
    }
}

s_vec2i PVC::randCity()
{
    return (s_vec2i){.x=randMinMax(m_min.x, m_max.x), .y=randMinMax(m_min.y, m_max.y)};
}

float PVC::dist(s_vec2i c1, s_vec2i c2)
{
    float dx=abs(c1.x-c2.x), dy=abs(c1.y-c2.y);
    return sqrt(dx*dx+dy*dy);
}

void shuffle(int *tab, int l)
{
    int a, b, temp;
    for(int i=0; i<l; i++)
    {
        a=randMinMax(i, l-1);
        temp=tab[i];
        tab[i]=tab[a];
        tab[a]=temp;
    }
    for(int i=0; i<l; i++)
    {
        if(tab[i]>l)
            std::cout << tab[i] << ", ";
    }
}
