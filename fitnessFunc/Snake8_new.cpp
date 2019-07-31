#include "Snake8_new.h"

Snake8_new::Snake8_new(s_vec2i max): Snake_new(max, false, 7, 3)
{
    m_activFoodGen=-1;
    m_rep.resize(3);
}

void Snake8_new::sensorUpdate()
{
    int head=m_snake.head.y*m_max.x+m_snake.head.x, d;
    int dx, dy, maxD;
    int x, y;
    int dir=((!m_snake.dir?3:m_snake.dir-1)*2);
    
    for(int i=0; i<7; i++, dir++)
    {
        dir%=8;
        x=(dir<3?-1:1);
        x=(dir==3 || dir==7)?0:x;
        y=0;
        if(dir<5 && dir>1)
            y=-1;
        else if((dir>5 && dir<=7) || dir==0)
            y=1;

        if(x)
            maxD=(dx=((x==1)?m_max.x-m_snake.head.x-1:m_snake.head.x));
        if(y)
            maxD=(dy=((y==1)?m_max.y-m_snake.head.y-1:m_snake.head.y));
        if(x && y)
            maxD=(dx<dy)?dx:dy;
        m_sensor[i]=maxD+1;
        for(int a=0; a<2; a++)
        {
            d=1;
            int t=head+x+y*m_max.x;
            if(a==1 && !m_bFood)
                m_sensor[7+a*7+i]=0;
            else
            {
                for(; d<maxD && m_viewTab[t]!=a+1; d++)
                {
                    t+=x+y*m_max.x;
                }
                if(m_viewTab[t]==a+1)
                    m_sensor[7+a*7+i]=d;
                else
                    m_sensor[7+a*7+i]=0;
            }
        }
    }
}

void Snake8_new::sensorDbg()
{
    for(int i=0; i<7; i++)
    {
        std::cout << "Wall " << "dir (" << i << ") dist: " << m_sensor[i] << std::endl;
        if(m_sensor[i+7])
            std::cout << "Body " << "dir (" << i << ") dist: " << m_sensor[i+7] << std::endl;
        if(m_sensor[i+14])
            std::cout << "Apple " << "dir (" << i << ") dist: " << m_sensor[i+14] << std::endl;
    }
}

int Snake8_new::evaluate(std::vector<double> &gene, int generation)
{
    if(m_activFoodGen>=0 && generation>=m_activFoodGen)
        set_food(true);
    evaluate(gene);
}

int Snake8_new::evaluate(std::vector<double> &gene)
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
            m_score+=m_moveNoEat/(m_applePoint/50);
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

void Snake8_new::step()
{
    fillViewTab();
    sensorUpdate();
    int rep=calc(m_sensor);
    m_rep[rep]++;
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
        {
            change_dir(1);
        }
        else if(rep==2)
        {
            change_dir(3);
        }
    }
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
