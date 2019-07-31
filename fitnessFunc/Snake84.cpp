#include "Snake84.h"
#include "../random.h"
#include <iostream>

Snake84::Snake84(s_vec2i max): m_max(max), NeuralNet_wrapper(24)
{
    addLayerFront(Layer(4));
    addLayerFront(Layer(16));
    m_nbWeight=0;
    std::list<Layer>::iterator it=m_layer.begin();
    NeuralNet::init();
    for(; it!=m_layer.end(); it++)
    {
        m_nbWeight+=it->get_nbNeuronWeight()*it->get_nbNeuron();
    }
    //m_scoreTab.resize(m_nbEvaluate);
    m_sensor.resize(24);
    snake_init();
    m_viewTab.resize(m_max.x*m_max.y);
}

void Snake84::snake_init()
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

void Snake84::sensorUpdate()
{
    int head=m_snake.head.y*m_max.x+m_snake.head.x, d;
    int dx, dy, maxD;
    int x, y;
    
    for(int i=0; i<8; i++)
    {
        x=(i<3?-1:1);
        x=(i==3 || i==7)?0:x;
        y=0;
        if(i<5 && i>1)
            y=-1;
        else if((i>5 && i<=7) || i==0)
            y=1;

        if(x)
            maxD=(dx=((x==1)?m_max.x-m_snake.head.x-1:m_snake.head.x));
        if(y)
            maxD=(dy=((y==1)?m_max.y-m_snake.head.y-1:m_snake.head.y));
        if(x && y)
            maxD=(dx<dy)?dx:dy;
        m_sensor[i]=maxD;
        for(int a=0; a<2; a++)
        {
            d=1;
            int t=head+x+y*m_max.x;
            if(a==1 && !m_bFood)
                m_sensor[8+a*8+i]=0;
            else
            {
                for(; d<maxD && m_viewTab[t]!=a+1; d++)
                {
                    t+=x+y*m_max.x;
                }
                if(m_viewTab[t]==a+1)
                    m_sensor[8+a*8+i]=d;
                else
                    m_sensor[8+a*8+i]=0;
            }
        }
    }
}

void Snake84::sensorDbg()
{
    for(int i=0; i<8; i++)
    {
        std::cout << "Wall " << "dir (" << i << ") dist: " << m_sensor[i] << std::endl;
        if(m_sensor[i+8])
            std::cout << "Body " << "dir (" << i << ") dist: " << m_sensor[i+8] << std::endl;
        if(m_sensor[i+16])
            std::cout << "Apple " << "dir (" << i << ") dist: " << m_sensor[i+16] << std::endl;
    }
}

int Snake84::evaluate(std::vector<double> &gene, int generation)
{
    if(m_activFoodGen>=0 && generation>=m_activFoodGen)
        set_food(true);
    evaluate(gene);
}

int Snake84::evaluate(std::vector<double> &gene)
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
            //m_score=m_score*(m_nbRight>m_nbLeft?(double)m_nbLeft/m_nbRight:(double)m_nbRight/m_nbLeft);
            m_score+=m_moveNoEat/5;
            finalScore+=m_score;
        }
        //std::cout << m_rep[0] << ", " << m_rep[1] << ", " << m_rep[2] << std::endl;
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

void Snake84::step()
{
    fillViewTab();
    sensorUpdate();
    int rep=calc(m_sensor);
    change_dir(rep);
    move();
    if(!m_bFood)
    {
        if(m_score%10==0)
            add_len();
        m_score++;
    }
    else if(m_bEat)
    {
        m_bEat=false;
        m_score+=m_applePoint-m_moveNoEat/10;
        m_moveNoEat=0;
        add_len();
        randApple();
    }
    else if(m_moveNoEat>(m_starving+m_snake.len))
        m_dead=true;
    else
        m_moveNoEat++;
}

void Snake84::randGenome(std::vector<double> &gene)
{
    for(int i=0; i<m_nbWeight; i++)
    {
        gene[i]=randMinMax(-10000, 10000)/10000.0;
    }
}

void Snake84::randApple()
{
    m_apple.x=randMinMax(3, m_max.x-4);
    m_apple.y=randMinMax(3, m_max.y-4);
}

void Snake84::set_food(bool b)
{
    m_bFood=b;
    if(m_bFood)
        randApple();
}

void Snake84::fillNet(std::vector<double> &gene)
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

void Snake84::move()
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

bool Snake84::fillViewTab()
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

void Snake84::init_body()
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

void Snake84::add_len(int l)
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

void Snake84::change_dir(int dir)
{
    if(!m_dead)
    {
        m_snake.dir=dir;
    }
}
