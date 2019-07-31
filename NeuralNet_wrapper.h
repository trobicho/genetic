#pragma once
#include "Fitness.h"
#include "../neuralNet/NeuralNet.h"

class NeuralNet_wrapper: public Fitness, protected NeuralNet
{
    public:
        NeuralNet_wrapper(int nbEntrie):NeuralNet(nbEntrie){};
        NeuralNet_wrapper(int nbEntrie, int nbNeuronOut, unsigned int nbHLayer=0):NeuralNet(nbEntrie, nbNeuronOut, nbHLayer){};
};
