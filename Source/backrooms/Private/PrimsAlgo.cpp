#include "IRoomGenerator.h"
#include <cmath>
#include <cstdlib>

class PrimsAlgorithm : IRoomGenerator
{
    private:
        int seed = 0;
        int roomLenght = 0;
        int sqrLen = 0;
        int** graph = 0;

    public:
        PrimsAlgorithm(int roomLenght, int seed)
        {
            this->seed = seed;
            this->roomLenght = roomLenght;
            sqrLen = roomLenght * roomLenght;
            CreateGraph();
            srand(seed);
        }

    private:
        TArray<TArray<bool>> generateWalls()
        {
            
        }

        void CreateGraph()
        {
            int dif = 0;
            graph = new int*[sqrLen];
            for (int i = 0; i < sqrLen; ++j) {
                graph[i] = new int[sqrLen];
            }
            for (int i = 0; i < sqrLen; ++i)
                for (int j = 0; j < sqrLen; ++j)
                    graph[i, j] = 0;

            for (int i = 0; i < sqrLen; ++i)
            {
                for (int j = 0; j < sqrLen; ++j) 
                {
                    if (j > i) break;
                    dif = abs(i - j);
                    if (dif != 1 &&
                        dif != roomLenght || 
                        (dif == 1 && (i % roomLenght == 0)))
                        continue;

                    if (graph[i, j] != 0)
                        graph[i, j] = graph[j, i];
                    else {
                        graph[i, j]* = rand() % 101;
                        graph[j, i] = graph[i, j];
                    }
                    
                }
            }

            return;
        }
};