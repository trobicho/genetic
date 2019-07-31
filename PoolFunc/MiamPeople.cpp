#include "MiamPeople.h"
        
MiamPeople::MiamPeople(): PeoplePool(), NeuralNet(3)
{
    addLayerFront(Layer(3));
    addLayerFront(Layer(8));
    NeuralNet::init();
    m_nbWeight=0;
    std::list<Layer>::iterator it=m_layer.begin();
    for(; it!=m_layer.end(); it++)
    {
        m_nbWeight+=it->get_nbNeuronWeight()*it->get_nbNeuron();
        it->set_kFactor(10);
    }
    set_nbGene(m_nbWeight);
    init();
}

void MiamPeople::init()
{
    fillNet();
    m_miam.hungry=300;
    m_miam.angle=0;
    m_miam.death=false;
    m_miam.speed=0;
    m_miam.viewAngle=3;
    m_score=0;
    m_bDeath=false;
}

void MiamPeople::move()
{
    double x=cos(m_miam.angle);
    double y=sin(m_miam.angle);

    m_miam.x+=x*m_miam.speed;
    m_miam.y+=y*m_miam.speed;
    double d=abs(x)*m_miam.speed+abs(y)*m_miam.speed;
    //m_miam.speed-=0.2;
    m_miam.speed=0.0;
    if(m_miam.speed<0.0)
        m_miam.speed=0.0;
    m_miam.hungry--;
}

void MiamPeople::move(double speed)
{
    double x=cos(m_miam.angle);
    double y=sin(m_miam.angle);

    m_miam.x+=x*speed;
    m_miam.y+=y*speed;
    m_miam.hungry--;
}

void MiamPeople::angleAdd(double a)
{
    double pi=3.1415926535;
    m_miam.angle+=a;
    if(m_miam.angle>pi)
        m_miam.angle=-pi;
    else if(m_miam.angle<-pi)
        m_miam.angle=pi;
}

void MiamPeople::fillNet()
{
    std::shared_ptr<std::vector<s_neuron>> neuronVec=NULL;
    std::list<Layer>::iterator it=m_layer.begin();
    int l=0;
    for(int g=0; it!=m_layer.end(); it++)
    {
        neuronVec=it->get_neuronVector();
        for(int i=0; i<neuronVec->size(); i++)
        {
            for(int j=0; g<m_nbWeight && j<neuronVec->at(i).weight.size(); j++)
            {
                neuronVec->at(i).weight[j]=m_gene[g];
                g++;
            }
        }
        l++;
    }
}

void MiamPeople::randGenome()
{
    for(int i=0; i<m_nbWeight; i++)
    {
        m_gene[i]=randMinMax(-5000, 5000)/10000.0;
    }
}
