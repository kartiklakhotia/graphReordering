#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ctime>
//#include <time.h>
#include <chrono>

#include "graph.h"
#include "sort.h"

using namespace std;

#define MAX_ITER 10
#define DEBUG
#undef DEBUG

void findOutDeg(graph*, int*);
void prIter(graph*, double*, double, int*, int);
double read_timer();

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Usage : %s <filename>\n", argv[0]);
        exit(1);
    }
    
    double dampingFactor = 0.85;

    // graph object
    graph G;

    // read csr file
    if (read_csr(argv[1], &G)==-1)
    {
        printf("couldn't read %s\n", argv[1]);
        exit(1);
    }

#ifdef DEBUG
    printf("%d %d\n", G.numVertex, G.numEdges);
#endif    
    // output Degree array
    int* outDeg = new int [G.numVertex]();
    findOutDeg(&G, outDeg);


    // initialize page rank attribute to 1
    initGraph (&G, outDeg);

    ///////////////////////////////////////
    /////// write algorithm here //////////
    int numIter = 0;
    double* temp = new double[G.numVertex]();

    bool converged = false;

	//warmup iterations//
    while (numIter < 2)
    {
        prIter(&G, temp, dampingFactor, outDeg, numIter++);
    }
    numIter = 0;

    clock_t start, end;
    start = clock();
//    struct timespec start, stop;
//    if (clock_gettime(CLOCK_REALTIME, &start) == -1){perror("clock gettime");}
//    double start = read_timer();
    while (numIter < MAX_ITER)
    {
        prIter(&G, temp, dampingFactor, outDeg, numIter++);
    }
    double time = read_timer() - start;
    end = clock();
//    if (clock_gettime(CLOCK_REALTIME, &stop) == -1){perror("clock gettime");}
//    double time = (stop.tv_sec - start.tv_sec) + ((double)(stop.tv_nsec - start.tv_nsec))/1e9;
    printf("%s, %lf\n", argv[1], (double)(end-start)/CLOCKS_PER_SEC);

	

//#ifdef DEBUG
	mergeSortWOkey (G.attr, 0, G.numVertex-1);
    FILE* fdump = fopen("dumpPR.txt", "w");
    if (fdump == NULL)
    {
        fputs("file error\n", stderr);
        exit(1);
    }
	int printVertices = (G.numVertex>1000) ? 1000 : G.numVertex;
	if (numIter%2==0)
	{
		for (int i=0; i<printVertices; i++)
			fprintf(fdump, "%lf\n", G.attr[i]);
	}
	else
	{
		for (int i=0; i<printVertices; i++)
			fprintf(fdump, "%lf\n", temp[i]);
	}
    fclose(fdump);
//#endif
    //////////////////////////////////////

    // free allocated memory//
    freeMem(&G);
    delete[] temp;
    delete[] outDeg;
    

    return 0;
}

void findOutDeg(graph* G, int* degArr)
{
    int i;
    for (i=0; i<G->numEdges; i++)
        degArr[G->EI[i]]++;
    return;
}

void prIter (graph* G, double* temp, double d, int* degArr, int numIter)
{
    if (numIter%2)
    {
        for (int i=0; i<G->numVertex; i++)
        {
            double tempVal = 0;
            for (int j=G->VI[i]; j<G->VI[i+1]; j++)
                tempVal += ((temp[G->EI[j]])/(degArr[G->EI[j]]));
            G->attr[i] = d + (1-d)*tempVal;
        }
    }
    else
    {
        for (int i=0; i<G->numVertex; i++)
        {
            double tempVal = 0;
            for (int j=G->VI[i]; j<G->VI[i+1]; j++)
                tempVal += ((G->attr[G->EI[j]])/(degArr[G->EI[j]]));
            temp[i] = d + (1-d)*tempVal;
        }
    }
}

double read_timer()
{
  static bool initialized = false;
  static struct timeval start;
  struct timeval end;
  if( !initialized ) {
    gettimeofday( &start, NULL );
    initialized = true;
  }
  
  gettimeofday( &end, NULL );
  
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}
