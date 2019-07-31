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
}s_body;

typedef struct
{
    s_vec2i head;
    std::vector<s_body> body;
    int dir, len, vLen;
}s_snake;

class Snake84: public NeuralNet_wrapper
{
    public:
        explicit Snake84(s_vec2i max);
        ~Snake84(){;}
        void snake_init();
        void fillNet(std::vector<double> &gene);
        void sensorUpdate();
        void sensorDbg();
        void step();
        void randApple();
        void set_food(bool);
        void init_body();
        void add_len(int l=1);

        int evaluate(std::vector<double> &gene, int generation);
        int evaluate(std::vector<double> &gene);
        bool get_sortInvert(){return true;}
        void randGenome(std::vector<double> &gene);
        void repairGenome(std::vector<double> &gene){};
        int get_nbGene(){return m_nbWeight;}
    
    protected:
        void move();
        void change_dir(int dir);
        bool fillViewTab();

        s_snake m_snake;
        s_vec2i m_max, m_apple;
        int m_basicLen=10;
        bool m_dead, m_bFood=false, m_bEat=false;
        int m_score;
        int m_applePoint=100;
        int m_nbWeight;
        int m_activFoodGen=20;
        int m_moveNoEat=0;
        int m_nbEvaluate=1;
        int m_starving=500;
        int m_nbMove;
        std::vector<int> m_sensor;
        std::vector<int> m_viewTab;
        //std::vector<int> m_scoreTab;
};
