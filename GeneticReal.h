#pragma once
#include "People.h"
#include "AbsGenetic.h"

class GeneticReal: public AbsGenetic
{
    public:
        GeneticReal(){};
        GeneticReal(Fitness *fitness, int nbPeople):GeneticReal(fitness, nbPeople, nbPeople*3){};
        GeneticReal(Fitness *fitness, int nbPeople, int peopleLen);
        ~GeneticReal();
        int get_bestScore(){return m_people[0]->get_score();}
        std::vector<double>& get_bestGene(){return m_people[0]->get_gene();}
        
        void nextGen();
        int crossing();
        int mutate();
        void parentSelect();
        void elitistSort();


    protected:
        std::vector<People*> m_people;
        Fitness *m_fitness;
        int m_nbStablePeople;
        int m_nbCrossing, m_nbMutate;
        int m_nbGenByPeople;
        
        std::vector<double> m_geneCopy;

    private:
        std::vector<People*> m_parent;
};
