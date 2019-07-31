#include "TankPeople.h"
        
TankPeople::TankPeople(): PeoplePool(), NeuralNet(4)
{
    addLayerFront(Layer(6));
    addLayerFront(Layer(24));
    NeuralNet::init();
    m_nbWeight=0;
    std::list<Layer>::iterator it=m_layer.begin();
    for(; it!=m_layer.end(); it++)
    {
        m_nbWeight+=it->get_nbNeuronWeight()*it->get_nbNeuron();
        it->set_kFactor(100);
    }
    set_nbGene(m_nbWeight);
    init();
}

void TankPeople::init()
{
    fillNet();
    m_tank.life=3;
    m_tank.angle=0;
    m_tank.gunAngle=0;
    m_tank.viewAngle=0.8;
    m_tank.death=false;
    m_tank.speed=0;
    m_tank.shootTimer=0;
    m_tank.hitbox.x=10;
    m_tank.hitbox.y=20;
    m_score=0;
    m_bDeath=false;
    m_nbBulletShoot=0;
    m_nbBulletHit=0;
}

void TankPeople::move()
{
    double x=cos(m_tank.angle);
    double y=sin(m_tank.angle);

    m_tank.x+=x*m_tank.speed;
    m_tank.y+=y*m_tank.speed;
    double d=abs(x)*m_tank.speed+abs(y)*m_tank.speed;
    //m_tank.speed-=0.2;
    m_tank.speed=0.0;
    if(m_tank.speed<0.0)
        m_tank.speed=0.0;
    if(m_tank.shootTimer)
        m_tank.shootTimer--;
}

void TankPeople::move(double speed)
{
    double x=cos(m_tank.angle);
    double y=sin(m_tank.angle);

    m_tank.x+=x*speed;
    m_tank.y+=y*speed;
}

void TankPeople::shoot()
{
    if(!m_tank.shootTimer)
    {
        m_tank.shootTimer=100;
        m_bShoot=true;
        m_nbBulletShoot++;
    }
}

void TankPeople::angleAdd(double a)
{
    double pi=3.1415926535;
    m_tank.angle+=a;
    if(m_tank.angle>pi)
        m_tank.angle=-pi;
    else if(m_tank.angle<-pi)
        m_tank.angle=pi;
}

void TankPeople::gunAngleAdd(double a)
{
    double pi=3.1415926535;
    m_tank.gunAngle+=a;
    if(m_tank.gunAngle>pi)
        m_tank.gunAngle=-pi;
    else if(m_tank.gunAngle<-pi)
        m_tank.gunAngle=pi;
}

void TankPeople::fillNet()
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

void TankPeople::randGenome()
{
    for(int i=0; i<m_nbWeight; i++)
    {
        m_gene[i]=randMinMax(-5000, 5000)/5000.0;
    }
}
