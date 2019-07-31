#include "Snake_new.h"

class Snake_genRand: public Snake_new
{
    public:
        explicit Snake_genRand(s_vec2i max);
        void step();
        
        int evaluate(std::vector<double> &gene, int generation);
        int evaluate(std::vector<double> &gene);
        void randAppleVec();
        void nextApple();
        void resetApple();

    private:
        int m_nbMove=0;
        int m_genLast=0;
        int m_curApple=0;
        std::vector<s_vec2i> m_appleVec;
};
