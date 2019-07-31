#pragma once
#include "../Fitness.h"

typedef struct
{
    int x, y;
}s_vec2i;

class PVC: public Fitness
{
    public:
        PVC(int nbCitys, s_vec2i min, s_vec2i max);
        ~PVC(){delete[] m_city; delete[] m_regenTab;}
        int evaluate(int *gene);
        void display(int *path);
        bool get_sortInvert(){return true;}
        void randGenome(int *gene);
        void repairGenome(int *gene);

    private:
        s_vec2i randCity();
        float dist(s_vec2i c1, s_vec2i c2);
        int *m_regenTab;

    protected:
        s_vec2i *m_city;
        s_vec2i m_min, m_max;
        int m_nbCitys;

};
