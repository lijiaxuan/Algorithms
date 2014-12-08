#ifndef ANT_H
#define ANT_H
#include "Tsp_Info.h"
#include <math.h>
#include <stdlib.h>
double rnd(int low,double upper);
int random(int upper);
class Ant
{
    public:
        Ant();
        virtual ~Ant();
        double m_dLength;
        double m_dShortest;
        int tabu[N_CITY_COUNT];//record the city he has traveled, in which is the order of the city
        void AddCity(int city);
        void Clear();
        void UpdateResult(Tsp_Info* Map);
        void Move(Tsp_Info* Map);
        void MoveToLast();
    protected:
    private:
        int ChooseNextCity(Tsp_Info* Map);
        double Prob[N_CITY_COUNT];//store the prob of go to next city
        int m_nCityCount;//the number of cities he has traveled
        int AllowedCity[N_CITY_COUNT];//the city he has not traveled
};

#endif // ANT_H
