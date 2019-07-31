#include "GeneticReal.h"
#include "fitnessFunc/Snake8_new.h"
#include <iostream>
#include <ctime>

int main(int argc, char **argv)
{
    s_vec2i max={10, 10};

    std::srand(time(NULL));
    Fitness *snake=new Snake8_new(max);
    GeneticReal genetic(snake, 100);

    static int maxS=0;
    for(int i=0; i<1000; i++)
    {
        genetic.nextGen();
        if(genetic.get_bestScore()>maxS)
        {
            std::cout << genetic.get_generation() << std::endl;
            std::cout << genetic.get_bestScore() << std::endl;
            maxS=genetic.get_bestScore();
            maxS=0;
        }
        if(i%100==0)
            std::cout << "generation: " << i << std::endl;
    }
}
