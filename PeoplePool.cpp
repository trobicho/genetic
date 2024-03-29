#include "PeoplePool.h"

void PeoplePool::copyGenome(PeoplePool *people)
{
    std::vector<double> &gene=people->get_gene();
    for(int i=0; i<m_nbGene; i++)
    {
        m_gene[i]=gene[i];
    }
}

void PeoplePool::copyGenome(std::vector<double> &gene)
{
    for(int i=0; i<m_nbGene; i++)
    {
        m_gene[i]=gene[i];
    }
}

void PeoplePool::mutate(void)//non universelle
{
    /*
    int temp, a=randMinMax(0, m_nbGene-1), b=randMinMax(0, m_nbGene-2);
    b=(b>a)?b+1:b;
    temp=m_gene[a];
    m_gene[a]=m_gene[b];
    m_gene[b]=temp;
    */
    int temp, a=randMinMax(0, m_nbGene-1);
    m_gene[a]+=randMinMax(-1000, 1000)/10000.0;
    //std::cout << randMinMax(-5000, 5000)/10000.0 << std::endl;
}
