#ifndef PROJECT_H
#define PROJECT_H
#include "Ant.h"
#include "Tsp_Info.h"
#include <time.h>
struct City
    {
        int num;
        int x;
        int y;
    };
class Project
{
    public:
        Project();
        virtual ~Project();
        double m_dLength;
        Ant Ants[N_ANTCOUNT];
        void UpdateTrial();
        void InitMap();
        void GetAnt();
        void StartSearch();
        Tsp_Info Map;
        int BestRoute[N_CITY_COUNT];
        City cc[N_CITY_COUNT];
    protected:
    private:
};

#endif // PROJECT_H
