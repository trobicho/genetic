#include "Tank.h"

Tank::Tank(int nbPeople): GeneticPool(nbPeople)
{
    for(int i=0; i<m_people.size(); i++){m_people[i]=new TankPeople();}
    for(int i=0; i<m_nbStablePeople; i++){m_people[i]->randGenome();}
    set_nbGenByPeople(m_people[0]->get_nbGene());
    m_map.w=500;
    m_map.h=500;
    m_sensor.resize(2);
}

void Tank::nextGen()
{
    for(int i=0; i<m_nbPeople; i++){m_people[i]->init(); m_people[i]->set_pos(randMinMax(0, m_map.w), randMinMax(0, m_map.h));}
}

void Tank::genFinish()
{
    m_bullet.clear();
    GeneticPool<TankPeople>::genFinish();
}

void Tank::evaluate()
{
    int nbNoDead=0;
    double tankAlpha;
    m_bNewGen=false;
    for(int i=0; i<m_nbPeople; i++)
    {
        if(!m_people[i]->get_death())
        {
            s_vec2d dirVec=m_people[i]->get_dirVector();
            m_sensor[0]=wallDist(m_people[i]->get_pos(), dirVec);
            m_sensor[1]=tankDist(m_people[i]->get_pos(), m_people[i]->get_gunAngle(), m_people[i]->get_viewAngle(), &tankAlpha);
            m_sensor[2]=tankAlpha;
            m_sensor[3]=m_people[i]->get_shootTimer();
            m_people[i]->calc(m_sensor);
            s_vec2d pos=m_people[i]->get_pos();
            if(pos.x<0 || pos.y<0 || pos.x>=m_map.w || pos.y>=m_map.h)
                m_people[i]->set_death(true);
            else
            {
                nbNoDead++;
                if(m_people[i]->get_shoot())
                    m_bullet.push_front(Bullet(pos, m_people[i]->get_gunAngle(), i));
            }
        }
    }
    physic();
    if(nbNoDead<=1)
    {
        genFinish();
        nextGen();
        m_bNewGen=true;
    }
}

void Tank::physic()
{
    std::list<Bullet>::iterator it=m_bullet.begin();
    while(it!=m_bullet.end())
    {
        it->move();
        s_vec2d pos=it->get_pos();

        if(pos.x<0 || pos.y<0 || pos.x>=m_map.w || pos.y>=m_map.h)
            it=m_bullet.erase(it);
        else
        {
        
            int id=it->get_id();
            bool bShoot=false;
            for(int i=0; i<m_nbPeople; i++)
            {
                if(!m_people[i]->get_death() && i!=id)
                {
                    s_vec2d posT=m_people[i]->get_pos();
                    s_vec2d hb=m_people[i]->get_hitbox();
                    if(pos.x>=posT.x-hb.x/2 && pos.y>=posT.y-hb.y/2 && pos.x<=posT.x+hb.x/2 && pos.y<=posT.y+hb.y/2)
                    {
                        //std::cout << "tank:" << id << " shoot:" << i << std::endl;
                        m_people[id]->hitTarget();
                        if(!m_people[i]->add_life(-1))
                            m_people[id]->add_score(30);
                        it=m_bullet.erase(it);
                        break;
                    }
                }
            }
            if(!bShoot)
                it++;
        }
    }
}

double Tank::tankDist(s_vec2d pos, double angle, double vAngle, double *angleMin)
{
    double pi=3.1415926535;
    double alpha, minD=-1;
    bool bMin=false;
    s_vec2d posT;
    for(int i=0; i<m_nbPeople; i++)
    {
        if(!m_people[i]->get_death())
        {
            posT=m_people[i]->get_pos();
            alpha=atan2(posT.y-pos.y, posT.x-pos.x)-angle;
            if(alpha>pi)
                alpha=-(pi-(alpha-pi));
            if(alpha<-pi)
                alpha=pi-(alpha-pi);
            if(alpha>-vAngle/2 && alpha<vAngle/2)
            {
                double d=dist(pos, posT);
                if(!bMin || minD>d)
                {
                    minD=d;
                    if(angleMin!=NULL)
                        *angleMin=alpha;
                    bMin=true;
                }
            }
        }
    }
    return minD;
}

double Tank::wallDist(s_vec2d pos, s_vec2d vec)
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

double Tank::dist(s_vec2d p1, s_vec2d p2)
{
    double dX=p1.x-p2.x, dY=p1.y-p2.y;
    return sqrt(dX*dX+dY*dY);
}
