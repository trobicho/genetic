#pragma once
#include "random.h"
#include "SortInvert.h"

class AbsGenetic
{
    public:
        AbsGenetic(){};
        AbsGenetic(int nbPeople):m_nbPeople(nbPeople){};

        int				get_generation(void){return m_generation;}
        void			set_nbPeople(int nb){m_nbPeople=nb;}
        virtual void	nextGen()=0;
        virtual int		get_bestScore()=0;

    protected:
        virtual int		crossing()=0;
        virtual int		mutate()=0;
        int				wheel_sigma();//class Selector
        int				sigma(int n);//class Selector ou Calcul
        
        int			m_generation=1;
        int			m_nbPeople;
        int			m_maxGen=100;
        int 		m_mutateDegre=2;
        int			m_nbStablePeople;
        SortInvert	*m_sort;
};
