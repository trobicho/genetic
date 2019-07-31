#pragma once
#include "AbsPeople.h"
#include "Fitness.h"
#include "random.h"

class PeoplePool: public AbsPeople<double>
{
    public:
        PeoplePool():AbsPeople(){};
        PeoplePool(int nbGene, int generation=1):AbsPeople(nbGene, generation){};
        void copyGenome(PeoplePool *people);
        void copyGenome(std::vector<double> &gene);

        //std::vector<double>& get_gene(){return m_gene;}
        void mutate();//non universelle
        
        virtual void randGenome()=0;
        virtual void init()=0;
};
