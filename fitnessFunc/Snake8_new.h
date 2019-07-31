#include "Snake_new.h"

class Snake8_new: public Snake_new
{
    public:
        explicit Snake8_new(s_vec2i max);
        void sensorUpdate();
        void sensorDbg();
        void step();
        
        int evaluate(std::vector<double> &gene, int generation);
        int evaluate(std::vector<double> &gene);
    private:
        std::vector<int> m_rep;
        int m_nbMove=0;
};
