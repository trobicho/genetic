#include "GeneticReal.h"
#include <iostream>

GeneticReal::GeneticReal(Fitness *fitness, int nbPeople, int peopleLen):AbsGenetic(nbPeople), m_fitness(fitness), m_nbStablePeople(nbPeople)
{
    m_nbGenByPeople=fitness->get_nbGene();
    m_people.resize(peopleLen);
    m_geneCopy.resize(m_nbGenByPeople);
    m_parent.resize(2);
    m_sort=new SortInvert();
    for(int i=0; i<m_people.size(); i++){m_people[i]=new People(m_fitness, m_nbGenByPeople, 2);}
    for(int i=0; i<m_nbPeople; i++){m_people[i]->randGenome();}
    //m_sort=(fitness->get_sortInvert()?new SortInvert():new Sort());
    m_nbCrossing=m_nbStablePeople/2;
    m_nbMutate=m_nbStablePeople;
}

GeneticReal::~GeneticReal()
{
    for(int i=0; i<m_people.size(); i++){delete m_people[i];}
    delete m_sort;
}

void GeneticReal::nextGen(void)
{
    for(int i=0; i<m_nbPeople; i++)
    {
        m_people[i]->evaluate(m_generation);
    }
    m_sort->sort(m_people, m_nbPeople);
    
    m_generation++;
    for(int i=0; i<m_nbCrossing && crossing()!=-1; i++){;}
    m_sort->sort(m_people, m_nbPeople);
    for(int i=0; i<m_nbMutate && mutate()!=-1; i++){;}
    m_sort->sort(m_people, m_nbPeople);
    //elitistSort();
    m_nbPeople=m_nbStablePeople;
}

int GeneticReal::crossing(void)
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

    int min=0, indexMin=0, t;
    for(int i=0; i<3; i++)
    {
        m_people[m_nbPeople+i]->set_generation(m_generation);
        t=m_people[m_nbPeople+i]->evaluate();
        if(!i || min>t)
        {
            min=t;
            indexMin=i;
        }
    }
    if(indexMin!=2)
    {
        People *temp=m_people[m_nbPeople+2];
        m_people[m_nbPeople+2]=m_people[m_nbPeople+indexMin];
        m_people[m_nbPeople+indexMin]=temp;
    }
    m_nbPeople+=2;
    return 0;
}

int GeneticReal::mutate(void)
{
    int p=wheel_sigma();
    if(m_nbPeople+1>=m_people.size())
        return -1;
    m_people[p]->mutate();
    m_people[m_nbPeople]->copyGenome(m_people[p]);
    m_people[m_nbPeople]->set_generation(m_generation);
    m_people[m_nbPeople]->mutate();
    m_people[m_nbPeople]->evaluate();
    m_nbPeople++;
    return 0;
}

void GeneticReal::elitistSort()
{
    People *temp;
    for(int i=m_nbStablePeople, p=1; i<m_nbPeople; i++, p++)
    {
        temp=m_people[p];
        m_people[p]=m_people[i];
        m_people[i]=temp;
    }
    m_nbPeople=m_nbStablePeople;
}

void GeneticReal::parentSelect(void)
{
    for(int i=0; i<2; i++){m_parent[i]=m_people[wheel_sigma()];}
}
