#include "GeneticPool.h"
#include <iostream>

template <class T>
GeneticPool<T>::GeneticPool(int nbPeople, int peopleLen):AbsGenetic(nbPeople), m_nbStablePeople(nbPeople)
{
    //m_nbGenByPeople=fitness->get_nbGene();
    m_people.resize(peopleLen);
    m_parent.resize(2);
    m_sort=new SortInvert();
    //for(int i=0; i<m_nbPeople; i++){m_people[i]->randGenome();}
    m_nbCrossing=m_nbStablePeople/2;
    m_nbMutate=m_nbStablePeople/2;
}

template <class T>
GeneticPool<T>::~GeneticPool()
{
    for(int i=0; i<m_people.size(); i++){delete m_people[i];}
    delete m_sort;
}


template <class T>
int GeneticPool<T>::crossing(void)
{
    parentSelect();
    if(m_nbPeople+3>=m_people.size())
        return -1;
    std::vector<double> &geneP1=m_parent[0]->get_gene();
    std::vector<double> &geneP2=m_parent[1]->get_gene();
    for(int i=0; i<m_nbGenByPeople; i++){m_geneCopy[i]=geneP1[i]/2.0+geneP2[i]/2.0;}
    m_people[m_nbPeople]->copyGenome(m_geneCopy);
    for(int i=0; i<m_nbGenByPeople; i++){m_geneCopy[i]=geneP1[i]*1.5-geneP2[i]/2.0;}
    m_people[m_nbPeople+1]->copyGenome(m_geneCopy);
    for(int i=0; i<m_nbGenByPeople; i++){m_geneCopy[i]=-geneP1[i]/2.0+geneP2[i]*1.5;}
    m_people[m_nbPeople+2]->copyGenome(m_geneCopy);

    int index=randMinMax(0, 2);
    if(index!=2)
    {
        T *temp=m_people[m_nbPeople+2];
        m_people[m_nbPeople+2]=m_people[m_nbPeople+index];
        m_people[m_nbPeople+index]=temp;
    }
    m_nbPeople+=2;
    return 0;
}

template <class T>
int GeneticPool<T>::mutate(void)
{
    int p=wheel_sigma();
    if(m_nbPeople+1>=m_people.size())
        return -1;
    m_people[p]->mutate();
    m_people[m_nbPeople]->copyGenome(m_people[p]);
    m_people[m_nbPeople]->set_generation(m_generation);
    m_people[m_nbPeople]->mutate();
    m_nbPeople++;
    return 0;
}

template <class T>
void GeneticPool<T>::elitistSort()
{
    T *temp;
    for(int i=m_nbStablePeople, p=1; i<m_nbPeople; i++, p++)
    {
        temp=m_people[p];
        m_people[p]=m_people[i];
        m_people[i]=temp;
    }
    m_nbPeople=m_nbStablePeople;
}

template <class T>
void GeneticPool<T>::parentSelect(void)
{
    for(int i=0; i<2; i++){m_parent[i]=m_people[wheel_sigma()];}
}
