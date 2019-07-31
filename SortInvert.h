#include "Sort.h"

class SortInvert: public Sort
{
    public:
        SortInvert(){};
        template<class T>
        void sort(std::vector<T*> &people, int nbPeoples)
        {
            int j;
            T *temp;
            for(int i=1; i<nbPeoples; i++)
            {
                temp=people[i];
                for(j=i; j>0 && people[j-1]->get_score()<temp->get_score(); people[j]=people[j-1], j--);
                people[j]=temp;
            }
        }
};
