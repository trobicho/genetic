#pragma once
#include "../GeneticPool.h"
#include "TankPeople.h"
#include <vector>
#include <list>

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
}s_map;

class Bullet
{
    public:
        Bullet(s_vec2d pos, double angle, int id, double speed=3):m_pos(pos), m_angle(angle), m_id(id), m_speed(speed){};
        s_vec2d get_pos(){return m_pos;}
        void move(){m_pos.x+=cos(m_angle)*m_speed; m_pos.y+=sin(m_angle)*m_speed;}
        int get_id(){return m_id;}

    private:
        s_vec2d m_pos;
        double m_angle, m_speed;
        int m_id;
};

class Tank: public GeneticPool<TankPeople>
{
    public:
        Tank(int nbPeople);
        void nextGen();
        void genFinish();
        void evaluate();
        double wallDist(s_vec2d pos, s_vec2d vec);
        double tankDist(s_vec2d pos, double angle, double vAngle, double *angleMin=NULL);
        double dist(s_vec2d p1, s_vec2d p2);
        void physic();

    protected:
        s_map m_map;
        std::vector<double> m_sensor;
        std::list<Bullet> m_bullet;
        bool m_bNewGen=false;
};
