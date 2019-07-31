#pragma once
#include <vector>

class Fitness
{
    public:
        Fitness(){};
        virtual int evaluate(std::vector<double> &gene, int generation)=0;
        virtual int evaluate(std::vector<double> &gene)=0;
        //virtual void display(int *gene)=0;
        virtual bool get_sortInvert()=0;
        virtual int get_nbGene()=0;
        virtual void randGenome(std::vector<double> &gene)=0;
        virtual void repairGenome(std::vector<double> &gene)=0;
};
