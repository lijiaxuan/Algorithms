#include "Project.h"

Project::Project()
{
    //ctor
    InitMap();
    m_dLength=10e9;
    //the city cordinate
    int i=0,j=0;
    Map.fp=fopen("log.txt","w");
    int x_Ary[51]=
    {
    37,49,52,20,40,21,17,31,52,51,
    42,31,5,12,36,52,27,17,13,57,
    62,42,16,8,7,27,30,43,58,58,
    37,38,46,61,62,63,32,45,59,5,
    10,21,5,30,39,32,25,25,48,56,
    30
    };

    int y_Ary[51]=
    {
    52,49,64,26,30,47,63,62,33,21,
    41,32,25,42,16,41,23,33,13,58,
    42,57,57,52,38,68,48,67,48,27,
    69,46,10,33,63,69,22,35,15,6,
    17,10,64,15,10,39,32,55,28,37,
    40
    };
    for(i=0;i<N_CITY_COUNT;i++)
    {
        cc[i].x=x_Ary[i];
        cc[i].y=y_Ary[i];
        cc[i].num=i;
        //--debug---fprintf(fp,"x is %d,y is %d pow is %f\n",cc[i].x,cc[i].y,sqrt(pow((cc[i].x-cc[i].y),2)));
    }
    //--debug---fprintf(fp,"\n");
    //calculate the shortest distance between two city
    for(i=0;i<N_CITY_COUNT;i++)
    {
        for(j=i;j<N_CITY_COUNT;j++)
        {
            Map.distance[j][i]=Map.distance[i][j]=(int)(sqrt(pow((cc[i].x-cc[j].x),2)+pow((cc[i].y-cc[j].y),2)));
            //fprintf(fp,"%f  ",Map.distance[i][j]);
        }
        //fprintf(fp,"\n");
    }
}

Project::~Project()
{
    //dtor
    //fclose(Map.fp);
}
//Update the environment pheromone
//Update when an ant has traveled all the cities(you can also Update when an ant move)
void Project::UpdateTrial()
{
    //calculate the changes of trial information
    int m=0;
    int n=0;
    int i=0,j=0;
    for(i=0;i<N_ANTCOUNT;i++)//calculate the pheromone every ant left. the shorter the route is the more pheromone
    {
        for(j=0;j<N_CITY_COUNT-1;j++)
        {
            m=Ants[i].tabu[j];
            n=Ants[i].tabu[j+1];
            Map.m_dDeltTrial[m][n]+=DB_Q/Ants[i].m_dLength;
            Map.m_dDeltTrial[n][m]=Map.m_dDeltTrial[m][n];
        }
        //the total pheromone from begin to end
        m=Ants[i].tabu[N_CITY_COUNT-1];
        n =Ants[i].tabu[0];

        Map.m_dDeltTrial[m][n]+=DB_Q/Ants[i].m_dLength;
        Map.m_dDeltTrial[n][m]=Map.m_dDeltTrial[m][n];
    }
    //new pheromone = old pheromone -pheromone volatilized
    for(i=0;i<N_CITY_COUNT;i++)
    {
        for(j=0;j<N_CITY_COUNT;j++)
        {
            Map.m_dTrial[i][j]=(Map.m_dTrial[i][j])*0.5+(Map.m_dDeltTrial[i][j]);
            Map.m_dDeltTrial[i][j]=0;
        }
    }
}
void Project::InitMap()
{
    int i=0,j=0;
    for(i=0;i<N_CITY_COUNT;i++)
    {
        for(j=0;j<N_CITY_COUNT;j++)
        {
            Map.m_dTrial[i][j]=1.0;
            Map.m_dDeltTrial[i][j]=0;
            //----debug-----printf("dTrial is %f",Map.m_dTrial[i][j]);
        }
    }
}
void Project::GetAnt()
{
    srand((unsigned)time(NULL));
    //select a begin city for every ant randomly
    int city=0,i=0;
    for(i=0;i<N_ANTCOUNT;i++)
    {
        city=random(N_CITY_COUNT);
        Ants[i].AddCity(city);
    }
}
void Project::StartSearch()
{
    int max_times=0;
    int i=0,j=0,k;
    double temp=0;
    int temptour[N_CITY_COUNT];
    double dbMin=0.0;
    while(max_times<N_IT_COUNT)
    {
        if(max_times==200)
        {k=0;}
        dbMin=1000000000.0;
        for(j=0;j<N_ANTCOUNT;j++)
         {
             for(i=0;i<N_CITY_COUNT-1;i++)
             Ants[j].Move(&Map);
         }
         for(j=0;j<N_ANTCOUNT;j++)
         {
             Ants[j].MoveToLast();
             Ants[j].UpdateResult(&Map);
         }
          temp=Ants[0].m_dLength;
          for(i=0;i<N_CITY_COUNT;i++)
          {
              temptour[i]=Ants[0].tabu[i];
          }
          for(j=0;j<N_ANTCOUNT;j++)
          {
              if(temp>Ants[j].m_dLength)
               {
                  temp=Ants[j].m_dLength;
                  //printf("j is %d",j);
                  for(i=0;i<N_CITY_COUNT;i++)
                  {
                      temptour[j]=Ants[j].tabu[i];
                      //printf("  %d ",temptour[i]);
                  }
                  //printf("\n");
                }
               if(dbMin>Ants[j].m_dLength)
               {
                  dbMin=Ants[j].m_dLength;
               }
          }
          if(temp<m_dLength)
          {
              m_dLength=temp;
              for(i=0;i<N_CITY_COUNT;i++)
              {
                  BestRoute[i]=temptour[i];
              }
          }
    printf("%d:%.0f\n",max_times,temp);
    UpdateTrial();
    for(j=0;j<N_ANTCOUNT;j++)
    {
        Ants[j].Clear();
    }
    max_times++;
    }
    printf("best route is %f\n",m_dLength);
    for(i=0;i<N_CITY_COUNT;i++)
    {
        printf("%d  ",BestRoute[i]);
    }
}
