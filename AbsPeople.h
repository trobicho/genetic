#pragma once
#include <vector>

template <typename T>
class AbsPeople
{
    public:
        AbsPeople():m_generation(1){};
        AbsPeople(int nbGene, int generation=1):m_nbGene(nbGene), m_generation(generation){m_gene.resize(m_nbGene);}

        int get_score(){return m_score;}
        void set_generation(int g){m_generation=g;}
        int get_generation(){return m_generation;}
        std::vector<T>& get_gene(){return m_gene;}
        void set_nbGene(int n){m_gene.resize((m_nbGene=n));}
        int get_nbGene(){return m_nbGene;}
        
    protected:
        std::vector<T> m_gene;
        int m_nbGene;
        int m_score;
        int m_generation;
};
