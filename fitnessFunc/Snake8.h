#include "Snake.h"

class Snake8: public Snake
{
    public:
        explicit Snake8(s_vec2i max);
        void sensorUpdate();
        void step();
        
        int evaluate(double *gene, int generation);
        int evaluate(double *gene);
};
