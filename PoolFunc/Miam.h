#pragma once
#include "../GeneticPool.h"
#include "MiamPeople.h"
#include <vector>

/*
typedef struct
{
    double x, y;
    bool 
}s_food
*/

typedef struct
{
    int w, h;
    std::vector<s_vec2d> food;
    int nbFood;
}s_map;

class Miam: public GeneticPool<MiamPeople>
{
    public:
        Miam(int nbPeople);
        void nextGen();
        void evaluate();
        double wallDist(s_vec2d pos, s_vec2d vec);
        double foodDist(s_vec2d pos, double angle, double vAngle, double *angleMin=NULL);
        double dist(s_vec2d p1, s_vec2d p2);
        void spawnFood();
        bool check_eat(MiamPeople *people);

    protected:
        s_map m_map;
        std::vector<double> m_sensor;
};
