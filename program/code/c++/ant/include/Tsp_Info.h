#ifndef TSP_INFO_H
#define TSP_INFO_H
#define N_ANTCOUNT 34 //ant num
#define N_CITY_COUNT 51 //city num
#define N_IT_COUNT 1300 //iteration num

#define DB_Q 100 //total pheromone
#define DB_ALPHA 1//the importance of pheromone
#define DB_BETA 3 //the probability of ant going to the place where there is more pheromone
#define DB_ROU 0.5 ;//the speed of volatilization
#include <stdio.h>

class Tsp_Info
{
    public:
        Tsp_Info();
        virtual ~Tsp_Info();
        double m_dDeltTrial[N_CITY_COUNT][N_CITY_COUNT];
        //a temp matrix ,the pheromone between cities,calculate
        double m_dTrial[N_CITY_COUNT][N_CITY_COUNT];//the pheromone between cities
        double distance[N_CITY_COUNT][N_CITY_COUNT];
        FILE* fp;
    protected:
    private:
};

#endif // TSP_INFO_H
