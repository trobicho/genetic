#include "Miam.h"

Miam::Miam(int nbPeople): GeneticPool(nbPeople)
{
    for(int i=0; i<m_people.size(); i++){m_people[i]=new MiamPeople();}
    for(int i=0; i<m_nbStablePeople; i++){m_people[i]->randGenome();}
    set_nbGenByPeople(m_people[0]->get_nbGene());
    m_map.w=200;
    m_map.h=200;
    m_sensor.resize(2);
    m_map.nbFood=0;
    m_map.food.resize(30);
}

void Miam::nextGen()
{
    for(int i=0; i<m_nbPeople; i++){m_people[i]->init(); m_people[i]->set_pos(randMinMax(0, m_map.w), randMinMax(0, m_map.h));}
    m_map.nbFood=0;
    spawnFood();
}

void Miam::evaluate()
{
    int rep;
    int nbNoDead=0;
    double foodAlpha;
    for(int i=0; i<m_nbPeople; i++)
    {
        if(!m_people[i]->get_death())
        {
            s_vec2d viewVec=m_people[i]->get_viewVector();
            m_sensor[0]=wallDist(m_people[i]->get_pos(), viewVec);
            m_sensor[1]=foodDist(m_people[i]->get_pos(), m_people[i]->get_angle(), m_people[i]->get_viewAngle(), &foodAlpha);
            m_sensor[2]=foodAlpha;
            rep=m_people[i]->calc(m_sensor);
            s_vec2d pos=m_people[i]->get_pos();
            check_eat(m_people[i]);
            if(pos.x<0 || pos.y<0 || pos.x>=m_map.w || pos.y>=m_map.h)
                m_people[i]->set_death(true);
            else if(m_people[i]->get_hungry()<=0)
                m_people[i]->set_death(true);
            nbNoDead++;
        }
    }
    if(nbNoDead<=1)
    {
        genFinish();
        nextGen();
    }
}

double Miam::foodDist(s_vec2d pos, double angle, double vAngle, double *angleMin)
{
    double pi=3.1415926535;
    double alpha, minD=-1;
    bool bMin=false;
    for(int i=0; i<m_map.nbFood; i++)
    {
        alpha=atan2(m_map.food[i].y-pos.y, m_map.food[i].x-pos.x)-angle;
        if(alpha>pi)
            alpha=-(pi-(alpha-pi));
        if(alpha<-pi)
            alpha=pi-(alpha-pi);
        if(alpha>-vAngle/2 && alpha<vAngle/2)
        {
            double d=dist(pos, m_map.food[i]);
            if(!bMin || minD>d)
            {
                minD=d;
                if(angleMin!=NULL)
                    *angleMin=alpha;
                bMin=true;
            }
        }
    }
    return minD;
}

double Miam::wallDist(s_vec2d pos, s_vec2d vec)
{
    double dX=0.0, dY=0.0;

    if(vec.x<0) 
    {
        dX=sqrt(pos.x*pos.x+pos.x*vec.y*pos.x*vec.y);
    }
    else if(vec.x>0) 
    {
        double d=m_map.w-pos.x;
        dX=sqrt(d*d+d*vec.y*d*vec.y);
    }
    if(vec.y<0) 
    {
        dY=sqrt(pos.y*pos.y+pos.y*vec.x*pos.y*vec.x);
    }
    else if(vec.y>0) 
    {
        double d=m_map.h-pos.y;
        dY=sqrt(d*d+d*vec.x*d*vec.x);
    }
    if(vec.x==0.0)
        return dY;
    else if(vec.y==0)
        return dX;
    return (dX>dY)?dY:dX;
}

double Miam::dist(s_vec2d p1, s_vec2d p2)
{
    double dX=p1.x-p2.x, dY=p1.y-p2.y;
    return sqrt(dX*dX+dY*dY);
}

void Miam::spawnFood()
{
    for(int i=m_map.nbFood; i<m_map.food.size(); i++)
    {
        m_map.food[i]=(s_vec2d){.x=randMinMax(0, m_map.w-2), .y=randMinMax(1, m_map.h-2)};
        m_map.nbFood++;
    }
}

bool Miam::check_eat(MiamPeople *people)
{
    s_vec2d pos=people->get_pos();
    for(int i=0; i<m_map.nbFood; i++)
    {
        if(m_map.food[i].x>=pos.x-2 && m_map.food[i].x<=pos.x+2 && m_map.food[i].y>=pos.y-2 && m_map.food[i].y<=pos.y+2)
        {
            people->add_hungry(200);
            people->add_score(100);
            m_map.food[i]=(s_vec2d){.x=randMinMax(0, m_map.w-2), .y=randMinMax(1, m_map.h-2)};
            return true;
        }
    }
    return false;
}
