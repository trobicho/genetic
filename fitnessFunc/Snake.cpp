#include "Snake.h"
#include "../random.h"
#include <iostream>

Snake::Snake(s_vec2i max, bool binaryNet, int nbSensor, int nbOutPerDir): m_max(max), m_binaryNet(binaryNet),\
m_nbOutPerDir(nbOutPerDir), NeuralNet_wrapper(nbSensor*nbOutPerDir)
{
    if(m_binaryNet)
        addLayerFront(Layer(1));
    else
        addLayerFront(Layer(3));
    addLayerFront(Layer(14));
    m_nbWeight=0;
    std::list<Layer>::iterator it=m_layer.begin();
    NeuralNet::init();
    for(; it!=m_layer.end(); it++)
    {
        m_nbWeight+=it->get_nbNeuronWeight()*it->get_nbNeuron();
        if(it==m_layer.begin())
            it->set_bias(100);
        else
            it->set_bias(20);
    }
    //m_scoreTab.resize(m_nbEvaluate);
    m_sensor.resize(nbSensor*nbOutPerDir);
    snake_init();
    m_viewTab.resize(m_max.x*m_max.y);
}

void Snake::snake_init()
{
    m_snake.head={.x=m_max.x/2, .y=m_max.y/2};
    m_snake.nbRot=0;
    m_snake.len=m_basicLen;
    m_snake.vLen=1;
    m_snake.dir=0;
    m_score=0;
    m_nbMove=0;
    m_blockRot=false;
    m_dead=false;
    m_moveNoEat=0;
    m_nbRight=0;
    m_nbLeft=0;
}

void Snake::sensorUpdate()
{
    int head=m_snake.head.y*m_max.x+m_snake.head.x, d;
    int maxD;
    for(int i=0; i<3; i++)
    {
        int dir=((!m_snake.dir?3:m_snake.dir-1)+i)%4;
        if(dir<2)
            maxD=(dir%2)?m_snake.head.y:m_snake.head.x;
        else
            maxD=((dir%2)?m_max.y-m_snake.head.y:m_max.x-m_snake.head.x)-1;
        d=0;
        int t=head;
        for(; d<maxD && (!m_viewTab[t] || !d); d++)
        {
            t+=dir%2?(dir-2)*m_max.x:dir-1;
        }
        m_sensor[i]=d;
        //m_sensor[i]=(m_viewTab[t]==2?-d:d);
        m_sensor[i+3]=(m_viewTab[t]==2?1:0);
        //if(m_sensor[i+3])
            //std::cout << "apple:" << dir << " dist:" << m_sensor[i] << std::endl;
    }
}

void Snake::sensorDbg()
{
    int halfS=m_sensor.size()/2;
    for(int i=0; i<halfS; i++)
    {
        if(m_sensor[i+halfS]>1)
            std::cout << "dir (" << i << ") dist: " << m_sensor[i] << " type [" << m_sensor[i+halfS] << "]" << std::endl;
    }
}

int Snake::evaluate(double *gene, int generation)
{
    if(m_activFoodGen>=0 && generation>=m_activFoodGen)
        set_food(true);
    evaluate(gene);
}

int Snake::evaluate(double *gene)
{
    int finalScore=0;
    if(m_bFood)
    {
        fillNet(gene);
        for(int i=0; i<m_nbEvaluate; i++)
        {
            randApple();
            snake_init();
            while(!m_dead)
            {
                step();
            }
            finalScore+=m_score;
        }
        return finalScore/m_nbEvaluate;
    }
    else
    {
        snake_init();
        fillNet(gene);
        while(!m_dead)
        {
            step();
        }
        return m_score;
    }
}

void Snake::step()
{
    fillViewTab();
    sensorUpdate();
    int rep=calc(m_sensor);
    if(m_binaryNet)
    {
        if(!rep)
            change_dir(1);
        else
            change_dir(3);
    }
    else
    {
        if(rep==1)
            change_dir(3);
        else if(rep==2)
            change_dir(1);
    }
    move();
    if(!m_bFood)
    {
        if(m_score%10==0)
            m_snake.len++;
    }
    else
    {
        if(m_score%20==0)
            m_snake.len++;
    }
    if(m_bEat)
    {
        m_bEat=false;
        m_score+=m_applePoint+(m_snake.len-m_basicLen)*((m_starving+m_snake.len)-m_moveNoEat);
        m_moveNoEat=0;
        m_snake.len+=10;
        randApple();
    }
    else if(m_moveNoEat>(m_starving+m_snake.len))
        m_dead=true;
    else
        m_moveNoEat++;
    m_score++;
}

void Snake::randGenome(double *gene)
{
    for(int i=0; i<m_nbWeight; i++)
    {
        gene[i]=randMinMax(1, 10000)/2000.0;
    }
}

void Snake::randApple()
{
    m_apple.x=randMinMax(3, m_max.x-4);
    m_apple.y=randMinMax(3, m_max.y-4);
}

void Snake::set_food(bool b)
{
    m_bFood=b;
    if(m_bFood)
        randApple();
}

void Snake::fillNet(double *gene)
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
                neuronVec->at(i).weight[j]=gene[g];
                g++;
                //std::cout << "(" << l << ", " << i << ", " << j << ")";
            }
        }
        l++;
    }
}

void Snake::move()
{
    m_snake.head.x+=(!(m_snake.dir%2))*(m_snake.dir-1);
    m_snake.head.y+=(m_snake.dir%2)*(m_snake.dir-2);
    if(m_snake.vLen<m_snake.len)
        m_snake.vLen++;
    m_blockRot=false;
    fillViewTab();
    if(m_bFood && m_snake.head.x==m_apple.x && m_snake.head.y==m_apple.y)
        m_bEat=true;
    m_nbMove++;
}

bool Snake::fillViewTab()
{
    int dir=m_snake.dir, r=0;
    s_vec2i v=m_snake.head;
    if(m_snake.head.x<0 || m_snake.head.y<0\
    || m_snake.head.x>=m_max.x || m_snake.head.y>=m_max.y)
        m_dead=true;
    else
    {
        for(int i=0; i<m_viewTab.size(); i++){m_viewTab[i]=0;}
        for(int i=0; i<m_snake.vLen; ++i) //carrement moyen d'accelerer le tout en passant jusqu'au prochain rot
        {
            v.x-=(!(dir%2))*(dir-1);
            v.y-=(dir%2)*(dir-2);
            if(i!=m_snake.vLen-1 && r<m_snake.nbRot && v.x==m_snake.rot[r].x && v.y==m_snake.rot[r].y)
            {
                dir=m_snake.rot[r].dir;
                r++;
            }
            if(m_snake.head.x==v.x && m_snake.head.y==v.y)
                m_dead=true;
            m_viewTab[v.x+v.y*m_max.x]=1;
        }
        m_snake.nbRot=r;
    }
    if(m_bFood)
        m_viewTab[m_apple.x+m_apple.y*m_max.x]=2;
    return m_dead;
}

void Snake::change_dir(int dir)
{
    if(!m_blockRot && ! m_dead)
    {
        if(m_snake.len>2)
        {
            m_snake.rot.resize(m_snake.nbRot+1);
            m_snake.rot.insert(m_snake.rot.begin(), (s_rot){.x=m_snake.head.x, .y=m_snake.head.y, .dir=m_snake.dir});
            m_snake.nbRot++;
        }
        m_snake.dir+=dir;
        m_snake.dir%=4;
    }
    m_blockRot=true;
}
