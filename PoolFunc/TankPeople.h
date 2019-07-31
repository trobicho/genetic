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
    int life;
    double angle;
    double gunAngle;
    double viewAngle;
    bool death;
    double speed;
    int shootTimer;
    s_vec2d hitbox;
}s_tank;

class TankPeople: public PeoplePool, protected NeuralNet
{
    public:
        TankPeople();
        void init();

        void copyGenome(PeoplePool *people){m_bGeneModif=true; PeoplePool::copyGenome(people);}
        void copyGenome(std::vector<double> &gene){m_bGeneModif=true; PeoplePool::copyGenome(gene);}
        void mutate(){m_bGeneModif=true; PeoplePool::mutate();}
        void randGenome();
        void set_pos(int x, int y){m_tank.x=x; m_tank.y=y;}
        void add_pos(int x, int y){m_tank.x+=x; m_tank.y+=y;}

        s_vec2d get_pos(){return (s_vec2d){.x=m_tank.x, .y=m_tank.y};}
        s_vec2d get_hitbox(){return m_tank.hitbox;}
        s_vec2d get_dirVector(){return (s_vec2d){.x=cos(m_tank.angle), .y=sin(m_tank.angle)};}
        double get_angle(){return m_tank.angle;}
        double get_gunAngle(){return m_tank.gunAngle;}
        double get_viewAngle(){return m_tank.viewAngle;}
        double get_speed(){return m_tank.speed;}
        int get_life(){return m_tank.life;}

        int add_life(int l){m_tank.life+=l; if(!m_tank.life){m_bDeath=true;} return m_tank.life;}
        int add_score(int s){m_score+=s;}
        bool get_death(){return m_bDeath;}
        void set_death(bool bDeath){m_bDeath=bDeath;}
        bool get_shoot(){return m_bShoot;}
        bool get_shootTimer(){return m_tank.shootTimer;}
        void hitTarget(){m_nbBulletHit++; m_score+=10;}

        //int get_score(){return m_score+(m_nbBulletShoot?((double)m_nbBulletHit/m_nbBulletShoot)*1000*m_nbBulletHit:0);}
        int get_score(){return m_score;}

        void move(double speed);

        template <typename T>
        int calc(T sensor)
        {
            int rep;
            m_bShoot=false;
            rep=NeuralNet::calc(sensor);
            std::vector<double> &result=m_layer.back().get_result();
            if(result[0]>0.5 || result[1]>0.5 || result[2]>0.5)
            {
                if(result[0]>result[1] && result[0]>result[2])
                    m_tank.speed=0.3;
                else
                    angleAdd((result[1]>result[2])?0.1:-0.1);
            }
            if(result[3]>0.5 || result[4]>0.5)
            {
                gunAngleAdd((result[3]>result[4])?0.05:-0.05);
            }
            if(result[5]>0.5)
                shoot();
            move();
            return rep;
        }

        s_tank m_tank;

    protected:
        void fillNet();
        void move();
        void shoot();
        void angleAdd(double a);
        void gunAngleAdd(double a);

    private:
        bool m_bGeneModif=true;
        int m_nbWeight;
        bool m_bDeath, m_bShoot=false;
        int m_nbBulletShoot, m_nbBulletHit;
};
