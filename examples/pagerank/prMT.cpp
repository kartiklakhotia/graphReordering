#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "graph.h"
#include "sort.h"

using namespace std;

typedef struct threadData
{
    int tid;
    int startVertex;
    int endVertex;
    graph* G;
    int* outDeg;
    double* srcArr;
    double* dstArr;
}threadData;

#define MAX_ITER 10
#define DEBUG
#undef DEBUG


int NUM_THREADS = 8;
pthread_barrier_t barrier;


void findOutDeg(graph*, int*);
void *prIter (void* threadArg)
{
    threadData* TD = (threadData *) threadArg; 
    graph* G = TD->G;
    int* degArr = TD->outDeg;
    double* srcArr = TD->srcArr;
    double* dstArr = TD->dstArr;
    int start = TD->startVertex;
    int end = TD->endVertex;
    int id = TD->tid;
    double d = 0.85;
    end = (end > G->numVertex) ? G->numVertex : end;
    double tempVal = 0; 
    int iter=0;
    while(iter < MAX_ITER)
    {
        if (iter%2)
        {
            for (int i=start; i<=end; i++)
            {
                double tempVal = 0; 
                int lastId = (i==G->numVertex) ? G->numEdges : G->VI[i+1];
                for (int j=G->VI[i]; j<lastId; j++)
                    tempVal += ((srcArr[G->EI[j]])/(degArr[G->EI[j]]));
                dstArr[i] = d + (1-d)*tempVal;
            }
        }
        else
        {
            for (int i=start; i<=end; i++)
            {
                double tempVal = 0; 
                int lastId = (i==G->numVertex) ? G->numEdges : G->VI[i+1];
                for (int j=G->VI[i]; j<lastId; j++)
                    tempVal += ((dstArr[G->EI[j]])/(degArr[G->EI[j]]));
                srcArr[i] = d + (1-d)*tempVal;
            }
        }
        iter++;
        pthread_barrier_wait (&barrier);
    }
     
}


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("Usage : %s <filename> <numThreads> \n", argv[0]);
        exit(1);
    }
    
    double dampingFactor = 0.85;
    int rc;
    NUM_THREADS = atoi(argv[2]);
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
    double* temp = new double[G.numVertex]();
    int* outDeg = new int [G.numVertex]();
    findOutDeg(&G, outDeg);



    // initialize page rank attribute to 1
    initGraph (&G, outDeg);
    pthread_t* threads = (pthread_t*) malloc (sizeof(pthread_t)*NUM_THREADS);
    threadData* TD = (threadData*) malloc (sizeof(threadData)*NUM_THREADS);
    pthread_barrier_init (&barrier, NULL, NUM_THREADS);

    int numEdgesPerThread = G.numEdges/NUM_THREADS;

    int vcount = -1;
    //static workload balancing
    for (int i=0; i<NUM_THREADS; i++)
    {
        TD[i].tid = i;
        TD[i].G = &G;
        TD[i].outDeg = outDeg;
        TD[i].startVertex = vcount + 1;
        TD[i].srcArr = temp;
        TD[i].dstArr = G.attr;
        vcount = vcount + 1;
        while(G.VI[vcount] < ((i+1)*numEdgesPerThread)) 
        {
            vcount++;
            if (vcount == G.numVertex-1)
                break;
        }
        TD[i].endVertex = vcount;
//        printf("%d, %d        %d, %d\n", TD[i].startVertex, TD[i].endVertex, G.VI[TD[i].startVertex], G.VI[TD[i].endVertex]); 
    }


    ///////////////////////////////////////
    /////// write algorithm here //////////
    ///////////////////////////////////////
    //clock_t start, end;
	struct timespec start, end; 
	double time;
	// measure the start time here
    int numIter = 0;

    bool converged = false;

    numIter = 0;

	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
    for (int i=0; i<NUM_THREADS; i++)
    {
        rc = pthread_create(&threads[i], NULL, prIter, (void *) &TD[i]);
    }
    for (int i=0; i<NUM_THREADS; i++)
    {
        rc = pthread_join(threads[i], NULL);
    }


	if( clock_gettime( CLOCK_REALTIME, &end) == -1 ) { perror("clock gettime");}		
	time = (end.tv_sec - start.tv_sec)+ (int)(end.tv_nsec - start.tv_nsec)/1e9;
    printf("%s, %lf\n", argv[1], time);

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
    //////////////////////////////////////

    // free allocated memory//
    pthread_barrier_destroy(&barrier);
    freeMem(&G);
    free(TD);
    free(threads);
    delete[] temp;
    delete[] outDeg;
    

    return 0;
}

void findOutDeg(graph* G, int* degArr)
{
    unsigned int i;
    for (i=0; i<G->numEdges; i++)
        degArr[G->EI[i]]++;
    return;
}


