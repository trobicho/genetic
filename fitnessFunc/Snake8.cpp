#include "Snake8.h"

Snake8::Snake8(s_vec2i max): Snake(max, false, 7)
{
    m_activFoodGen=3;
}

void Snake8::sensorUpdate()
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
        d=0;
        int t=head;
        for(; d<maxD && (!m_viewTab[t]); d++)
        {
            t+=x+y*m_max.x;
        }
        m_sensor[i]=d;
        //m_sensor[i*2+1]=m_viewTab[t]==2?1:0;
        m_sensor[i+7]=(m_viewTab[t]==2)?10:0;
        //m_sensor[i+7]=(m_viewTab[t]==2)?10:m_viewTab[t];
        //if(m_viewTab[t]==2)
            //std::cout << "apple:" << dir << " dist:" << m_sensor[i] << std::endl;
    }
}

int Snake8::evaluate(double *gene, int generation)
{
    if(m_activFoodGen>=0 && generation>=m_activFoodGen)
        set_food(true);
    evaluate(gene);
}

int Snake8::evaluate(double *gene)
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
            m_score+=(((double)m_nbMove/m_starving)*m_applePoint)/2;
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

void Snake8::step()
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
        {
            change_dir(1);
            m_nbRight++;
        }
        else if(rep==2)
        {
            change_dir(3);
            m_nbLeft++;
        }
    }
    move();
    if(!m_bFood)
    {
        if(m_score && m_score%10)
            m_snake.len++;
        m_score++;
    }
    else if(m_bEat)
    {
        m_bEat=false;
        m_score+=m_applePoint;//(m_starving+m_snake.len)-m_moveNoEat;
        m_moveNoEat=0;
        m_snake.len+=5;
        randApple();
    }
    else if(m_moveNoEat>(m_starving+m_snake.len))
        m_dead=true;
    else
        m_moveNoEat++;
}
