#include "Snake_new.h"
#include <iostream>

Snake_new::Snake_new(s_vec2i max, bool binaryNet, int nbSensor, int nbOutPerDir): m_max(max), m_binaryNet(binaryNet),\
m_nbOutPerDir(nbOutPerDir), NeuralNet_wrapper(nbSensor*nbOutPerDir)
{
    if(m_binaryNet)
        addLayerFront(Layer(1));
    else
        addLayerFront(Layer(3));
    addLayerFront(Layer(14));
    m_nbWeight=0;
    NeuralNet::init();
    std::list<Layer>::iterator it=m_layer.begin();
    for(; it!=m_layer.end(); it++)
    {
        m_nbWeight+=it->get_nbNeuronWeight()*it->get_nbNeuron();
        it->set_kFactor(10);
    }
    //m_scoreTab.resize(m_nbEvaluate);
    m_sensor.resize(nbSensor*nbOutPerDir);
    snake_init();
    m_viewTab.resize(m_max.x*m_max.y);
}

void Snake_new::snake_init()
{
    m_snake.head={.x=m_max.x/2, .y=m_max.y/2};
    m_snake.len=m_basicLen;
    m_snake.vLen=1;
    init_body();
    m_snake.dir=0;
    m_score=0;
    m_dead=false;
    m_moveNoEat=0;
    m_nbMove=0;
}

void Snake_new::sensorUpdate()
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
        m_sensor[i]=maxD;
        for(int a=0; a<2; a++)
        {
            if(a==1 && !m_bFood)
                m_sensor[6+i]=0;
            else
            {
                int t=head;
                for(; d<maxD && m_viewTab[t]!=a+1; d++)
                {
                    t+=dir%2?(dir-2)*m_max.x:dir-1;
                }
                if(m_viewTab[t]==a+1)
                    m_sensor[3+a*3+i]=d;
                else
                    m_sensor[3+a*3+i]=0;
            }
        }
    }
}

void Snake_new::sensorDbg()
{
    int halfS=m_sensor.size()/2;
    for(int i=0; i<halfS; i++)
    {
        if(m_sensor[i+halfS]>1)
            std::cout << "dir (" << i << ") dist: " << m_sensor[i] << " type [" << m_sensor[i+halfS] << "]" << std::endl;
    }
}

int Snake_new::evaluate(std::vector<double> &gene, int generation)
{
    if(m_activFoodGen>=0 && generation>=m_activFoodGen)
        set_food(true);
    evaluate(gene);
}

int Snake_new::evaluate(std::vector<double> &gene)
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

void Snake_new::step()
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
            add_len(1);
    }
    else if(m_bEat)
    {
        m_bEat=false;
        m_score+=m_applePoint+(m_snake.len-m_basicLen)*((m_starving+m_snake.len)-m_moveNoEat);
        m_moveNoEat=0;
        add_len(5);
        randApple();
    }
    else if(m_moveNoEat>(m_starving+m_snake.len))
        m_dead=true;
    else
        m_moveNoEat++;
    m_score++;
}

void Snake_new::randGenome(std::vector<double> &gene)
{
    for(int i=0; i<m_nbWeight; i++)
    {
        gene[i]=randMinMax(-5000, 5000)/10000.0;
    }
}

void Snake_new::randApple()
{
    m_apple.x=randMinMax(3, m_max.x-4);
    m_apple.y=randMinMax(3, m_max.y-4);
}

void Snake_new::set_food(bool b)
{
    m_bFood=b;
    if(m_bFood)
        randApple();
}

void Snake_new::fillNet(std::vector<double> &gene)
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

void Snake_new::move()
{
    m_snake.head.x+=(!(m_snake.dir%2))*(m_snake.dir-1);
    m_snake.head.y+=(m_snake.dir%2)*(m_snake.dir-2);
    s_body temp=m_snake.body[0], temp2;
    m_snake.body[0]=(s_body){.x=m_snake.head.x, .y=m_snake.head.y, .dir=m_snake.dir};
    for(int i=1; i<m_snake.len; i++)
    {
        temp2=m_snake.body[i];
        m_snake.body[i]=temp;
        temp=temp2;
    }
    if(m_snake.vLen<m_snake.len)
        m_snake.vLen++;
    fillViewTab();
    if(m_bFood && m_snake.head.x==m_apple.x && m_snake.head.y==m_apple.y)
        m_bEat=true;
    m_nbMove++;
}

bool Snake_new::fillViewTab()
{
    if(m_snake.head.x<0 || m_snake.head.y<0\
    || m_snake.head.x>=m_max.x || m_snake.head.y>=m_max.y)
        m_dead=true;
    else
    {
        for(int i=0; i<m_viewTab.size(); i++){m_viewTab[i]=0;}
        for(int i=0; i<m_snake.vLen; ++i) //carrement moyen d'accelerer le tout en passant jusqu'au prochain rot
        {
            if(i && m_snake.head.x==m_snake.body[i].x && m_snake.head.y==m_snake.body[i].y)
                m_dead=true;
            m_viewTab[m_snake.body[i].x+m_snake.body[i].y*m_max.x]=1;
        }
    }
    if(m_bFood)
        m_viewTab[m_apple.x+m_apple.y*m_max.x]=2;
    return m_dead;
}

void Snake_new::init_body()
{
    m_snake.body.resize(m_snake.len);
    s_vec2i v=m_snake.head;
    int dir=m_snake.dir;
    for(int i=0; i<m_snake.len; i++)
    {
        m_snake.body[i].x=v.x;
        m_snake.body[i].y=v.y;
        m_snake.body[i].dir=dir;
        v.x-=(!(dir%2))*(dir-1);
        v.y-=(dir%2)*(dir-2);
    }
}

void Snake_new::add_len(int l)
{
    m_snake.body.resize(m_snake.len+l);
    for(int i=m_snake.len; i<m_snake.len+l; i++)
    {
        if(!i && !m_snake.len)
            m_snake.body[i]=(s_body){.x=m_snake.head.x, .y=m_snake.head.y, .dir=m_snake.dir};
        else
            m_snake.body[i]=m_snake.body[i-1];
    }
    m_snake.len+=l;
}

void Snake_new::change_dir(int dir)
{
    if(!m_dead)
    {
        m_snake.dir+=dir;
        m_snake.dir%=4;
    }
}
