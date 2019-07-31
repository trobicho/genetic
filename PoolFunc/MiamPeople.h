#pragma once
#include "../PeoplePool.h"
#include "../../neuralNet/NeuralNet.h"
#include <cmath>
#include <iostream>

typedef struct
{
    int x, y;
}s_vec2i;

typedef struct
{
    double x, y;
}s_vec2d;

typedef struct
{
    double x, y;
    int hungry;
    double angle;
    double viewAngle;
    bool death;
    double speed;
}s_miam;

class MiamPeople: public PeoplePool, protected NeuralNet
{
    public:
        MiamPeople();
        void init();

        void copyGenome(PeoplePool *people){m_bGeneModif=true; PeoplePool::copyGenome(people);}
        void copyGenome(std::vector<double> &gene){m_bGeneModif=true; PeoplePool::copyGenome(gene);}
        void mutate(){m_bGeneModif=true; PeoplePool::mutate();}
        void randGenome();
        void set_pos(int x, int y){m_miam.x=x; m_miam.y=y;}
        void add_pos(int x, int y){m_miam.x+=x; m_miam.y+=y;}
        s_vec2d get_pos(){return (s_vec2d){.x=m_miam.x, .y=m_miam.y};}
        s_vec2d get_viewVector(){return (s_vec2d){.x=cos(m_miam.angle), .y=sin(m_miam.angle)};}
        double get_angle(){return m_miam.angle;}
        double get_viewAngle(){return m_miam.viewAngle;}
        double get_speed(){return m_miam.speed;}
        int get_hungry(){return m_miam.hungry;}
        int add_hungry(int h){m_miam.hungry+=h;}
        int add_score(int s){m_score+=s;}
        void move();
        void move(double speed);
        void angleAdd(double a);
        bool get_death(){return m_bDeath;}
        void set_death(bool bDeath){m_bDeath=bDeath;}

        template <typename T>
        int calc(T sensor)
        {
            int rep;
            rep=NeuralNet::calc(sensor);
            switch(rep)
            {
                case 0:
                    m_miam.speed=1.5;
                    break;
                case 1:
                    angleAdd(0.1);
                    break;
                case 2:
                    angleAdd(-0.1);
                    break;
                default:
                    std::cout << "rep=" << rep << std::endl;
                    break;
            }
            move();
            return rep;
        }

        s_miam m_miam;

    protected:
        void fillNet();

    private:
        bool m_bGeneModif=true;
        int m_nbWeight;
        bool m_bDeath;
};
