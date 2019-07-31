#include "Snake_genRand.h"

Snake_genRand::Snake_genRand(s_vec2i max): Snake_new(max, false, 7, 3)
{
    m_activFoodGen=0;
    m_appleVec.resize(1000);
}

int Snake_genRand::evaluate(std::vector<double> &gene, int generation)
{
    if(m_activFoodGen>=0 && generation>=m_activFoodGen)
        set_food(true);
    if(m_bFood && (m_genLast<generation || !m_genLast))
    {
        randAppleVec();
        m_genLast=generation;
    }
    evaluate(gene);
}

int Snake_genRand::evaluate(std::vector<double> &gene)
{
    int finalScore=0;
    if(m_bFood)
    {
        fillNet(gene);
        for(int i=0; i<m_nbEvaluate; i++)
        {
            resetApple();
            snake_init();
            while(!m_dead)
            {
                step();
            }
            //m_score=m_score*(m_nbRight>m_nbLeft?(double)m_nbLeft/m_nbRight:(double)m_nbRight/m_nbLeft);
            m_score+=m_moveNoEat/(m_applePoint/50);
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

void Snake_genRand::step()
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
        nextApple();
    }
    else if(m_moveNoEat>(m_starving+m_snake.len))
        m_dead=true;
    else
        m_moveNoEat++;
}

void Snake_genRand::randAppleVec()
{
    for(int i=0; i<m_appleVec.size(); ++i)
    {
        m_appleVec[i].x=randMinMax(3, m_max.x-4);
        m_appleVec[i].y=randMinMax(3, m_max.y-4);
    }
    m_apple=m_appleVec[0];
    m_curApple=0;
}

void Snake_genRand::nextApple()
{
    if(++m_curApple%1000==0)
        m_curApple=0;
    m_apple=m_appleVec[m_curApple];
}

void Snake_genRand::resetApple()
{
    m_curApple=0;
    m_apple=m_appleVec[m_curApple];
}
