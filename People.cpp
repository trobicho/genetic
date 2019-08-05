#include "People.h"
#include <iostream>

void People::copyGenome(People *people)
{
    std::vector<double> &gene=people->get_gene();
    for(int i=0; i<m_nbGene; i++)
    {
        m_gene[i]=gene[i];
    }
}

void People::copyGenome(std::vector<double> &gene)
{
    for(int i=0; i<m_nbGene; i++)
    {
        m_gene[i]=gene[i];
    }
}

void People::mutate(void)//non universelle
{
    /*
    int temp, a=randMinMax(0, m_nbGene-1), b=randMinMax(0, m_nbGene-2);
    b=(b>a)?b+1:b;
    temp=m_gene[a];
    m_gene[a]=m_gene[b];
    m_gene[b]=temp;
    */
    int temp, a;
	a = randMinMax(0, m_nbGene-1);
    m_gene[a] *= randMinMax(500, 1500)/1000.0;
    //std::cout << randMinMax(-5000, 5000)/10000.0 << std::endl;
}
