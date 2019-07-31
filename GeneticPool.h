#pragma once
#include "PeoplePool.h"
#include "AbsGenetic.h"
#include <iostream>
#include <cmath>

template <class T>
class GeneticPool: public AbsGenetic
{
    public:
        GeneticPool(){};
        GeneticPool(int nbPeople):GeneticPool(nbPeople, nbPeople*3){};
        GeneticPool(int nbPeople, int peopleLen);
        ~GeneticPool();
        void set_nbGenByPeople(int nb){m_geneCopy.resize((m_nbGenByPeople=nb));}
        //int get_bestScore(){return m_people[0]->get_score();}
        //std::vector<double>& get_bestGene(){return m_people[0]->get_gene();}
        int get_bestScore(){return 0;}
        std::vector<double>& get_bestGene(){;}
        
        virtual void nextGen()=0;
        void genFinish();
        int crossing();
        int mutate();
        double mutateFunc(double a);
        void parentSelect();
        void elitistSort();
        virtual void evaluate()=0;


    protected:
        std::vector<T*> m_people;
        int m_nbStablePeople;
        int m_nbCrossing, m_nbMutate;
        int m_nbGenByPeople;
        int m_mutateProb;
        
        std::vector<double> m_geneCopy;

    private:
        std::vector<T*> m_parent;
};

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
    m_mutateProb=10;
}

template <class T>
GeneticPool<T>::~GeneticPool()
{
    for(int i=0; i<m_people.size(); i++){delete m_people[i];}
    delete m_sort;
}

template <class T>
void GeneticPool<T>::genFinish()
{
    m_sort->sort(m_people, m_nbPeople);
    std::cout << m_people[0]->get_score() << std::endl;
    m_generation++;
    for(int i=0; i<m_nbCrossing && crossing()!=-1; i++){;}
    //for(int i=0; i<m_nbMutate && mutate()!=-1; i++){;}
    m_nbPeople=m_nbStablePeople;
    mutate();
    elitistSort();
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
    for(int i=0; i<m_nbPeople; i++)
    {
        if(randMinMax(1, m_mutateProb)==1)
        {
            std::vector<double> &gene=m_people[i]->get_gene();
            //for(int i=0; i<m_nbGenByPeople; i++){m_geneCopy[i]=geneP1[i];}
            int g=randMinMax(0, m_nbGenByPeople-1);
            std::cout << gene[g] << ", ";
            gene[g]=(randMinMax(0, 1)?gene[g]-mutateFunc(gene[g]+2):gene[g]+mutateFunc(2-gene[g]));
            std::cout << gene[g] << std::endl;
        }

    }
}

template <class T>
double GeneticPool<T>::mutateFunc(double a)
{
    double r=randMinMax(0, 1000)/1000.0;
    return a*r*pow((1.0-m_generation/(double)m_maxGen), m_mutateDegre);
}

/*
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
*/

template <class T>
void GeneticPool<T>::elitistSort()
{
    T *temp;
    for(int i=m_nbStablePeople, p=3; i<m_nbPeople; i++, p++)
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
