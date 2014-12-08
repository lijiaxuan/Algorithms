#include "Ant.h"
#include <windows.h>
Ant::Ant()
{
    //ctor
    int i=0;
    m_dLength=m_dShortest=0;
    m_nCityCount=0;
    for(;i<N_CITY_COUNT;i++)
    {
        AllowedCity[i]=1;
        Prob[i]=0;
    }
}

Ant::~Ant()
{
    //dtor
}
//called when there is only one city he hasn't traveled
void Ant::MoveToLast()
{
    int i;
    for(i=0;i<N_CITY_COUNT;i++)
    {
        if(AllowedCity[i]==1)
        {
            AddCity(i);
            break;
        }
    }
}
//clear the data after the ant has traveled all the city
void Ant::Clear()
{
    int i;
    m_dLength=0;
    for(i=0;i<N_CITY_COUNT;i++)
    {
        Prob[i]=0;
        AllowedCity[i]=1;
    }
    i=tabu[N_CITY_COUNT-1];
    m_nCityCount=0;
    AddCity(i);
}
//add a city to the matrix he has traveled and change the flag in the matrix he does not traveled
void Ant::AddCity(int city)
{
    tabu[m_nCityCount]=city;
    m_nCityCount++;
    AllowedCity[city]=0;
}
int Ant::ChooseNextCity(Tsp_Info* Map)
{
    //Update the probability of path selection
    //select a path from tabu[m_nCityCount-1] to next
    int j=10000,i;
    double temp=0.0;
    int curCity=tabu[m_nCityCount-1];//Current City
    double tmp[N_CITY_COUNT];
    memset(tmp,0,N_CITY_COUNT);
    //calculate the sum pheromone between the current city and the city he can go
    for(i=0;i<N_CITY_COUNT;i++)
    {
        if(AllowedCity[i]==1)
        {
            tmp[i]=pow((1.0/Map->distance[curCity][i]),DB_BETA)*pow((Map->m_dTrial[curCity][i]),DB_ALPHA);
            //printf("tmp is %f\n",tmp[i]);
            temp=temp+tmp[i];
        }
    }
    //calculate the prob of next cities
    double sel=0;
    for(i=0;i<N_CITY_COUNT;i++)
    {
        if(AllowedCity[i]==1)
        {
            Prob[i]=tmp[i]/temp;
            sel+=Prob[i];
        }
        else
        {
            Prob[i]=0;
        }
    }
    //wheel method
    double mRate=rnd(0,sel);
    double mSelect=0;
    for(i=0;i<N_CITY_COUNT;i++)
    {
        if(AllowedCity[i]==1)
        {
            mSelect+=Prob[i];
        }
        if(mSelect>=mRate)
        {
            j=i;
            break;
        }
    }
    //temp=0.0
    if(j==10000)
    {
        for(i=0;i<N_CITY_COUNT;i++)
        {
            if(AllowedCity[i]==1)
            {
                j=i;
                break;
            }
        }
    }
    return j;
}
void Ant::Move(Tsp_Info* Map)
{
    int n=ChooseNextCity(Map);
    AddCity(n);
}
//get a random number in given range
double rnd(int low,double upper)
{
    double p=((rand()/(double)RAND_MAX)*((upper)-(low))+(low));
    return p;
}

//get a random number less than upper
int random(int upper)
{
    return ((rand()%upper));
}
//calculate the sum length after travel all the cities
void Ant::UpdateResult(Tsp_Info* Map)
{
    int i=0,j=0;
    for(i=0;i<N_CITY_COUNT-1;i++)
    {
        m_dLength+=Map->distance[tabu[i]][tabu[i+1]];
    }
    m_dLength+=Map->distance[tabu[N_CITY_COUNT-1]][tabu[0]];
    //fprintf(Map->fp,"m_dLength is %d \n",m_dLength);
}
