#pragma once
#include <vector>
#include "../NeuralNet_wrapper.h"

typedef struct
{
    int x, y;
}s_vec2i;

typedef struct
{
    int x, y, dir;
}s_rot;

typedef struct
{
    s_vec2i head;
    std::vector<s_rot> rot;
    int dir, nbRot, len, vLen;
}s_snake;

class Snake: public NeuralNet_wrapper
{
    public:
        explicit Snake(s_vec2i max, bool binaryNet=false, int nbSensor=3, int nbOutPerDir=2);
        ~Snake(){;}
        void snake_init();
        void fillNet(double *gene);
        void sensorUpdate();
        void sensorDbg();
        void step();
        void randApple();
        void set_food(bool);

        int evaluate(double *gene, int generation);
        int evaluate(double *gene);
        bool get_sortInvert(){return true;}
        void randGenome(double *gene);
        void repairGenome(double *gene){};
        int get_nbGene(){return m_nbWeight;}
    
    protected:
        void move();
        void change_dir(int dir);
        bool fillViewTab();

        s_snake m_snake;
        s_vec2i m_max, m_apple;
        int m_basicLen=10;
        bool m_blockRot=false, m_dead, m_bFood=false, m_bEat=false;
        int m_score, m_nbMove;
        int m_applePoint=100;
        int m_nbWeight;
        int m_activFoodGen=-1;
        int m_moveNoEat=0;
        int m_nbEvaluate=1;
        bool m_binaryNet=false;
        int m_starving=500;
        int m_nbOutPerDir=2;
        std::vector<int> m_sensor;
        std::vector<int> m_viewTab;
        int m_nbRight, m_nbLeft;
        //std::vector<int> m_scoreTab;
};
