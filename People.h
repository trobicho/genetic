#pragma once
#include "AbsPeople.h"
#include "Fitness.h"
#include "random.h"

class People: public AbsPeople<double>
{
    public:
        People():AbsPeople(){};
        People(Fitness *fitness, int nbGene, int generation=1): AbsPeople(nbGene, generation), m_fitness(fitness){};
        void copyGenome(People *people);
        void copyGenome(std::vector<double> &gene);

        int evaluate(int generation){return (m_score=m_fitness->evaluate(m_gene, generation));}
        int evaluate(){return (m_score=m_fitness->evaluate(m_gene));}
        //bool get_sortInvert(){return m_fitness->get_sortInvert();}
        //std::vector<double>& get_gene(){return m_gene;}
        
        void randGenome(){m_fitness->randGenome(m_gene);}
        //void crossing(People **parent, int nbParents);
        void mutate();//non universelle
        std::vector<T>& get_gene(){return m_gene;}
        void set_nbGene(int n){m_gene.resize((m_nbGene=n));}
        int get_nbGene(){return m_nbGene;}

    private:
        Fitness *m_fitness;
};
