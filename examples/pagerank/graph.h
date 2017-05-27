#include <iostream>
#include <stdio.h>
#include <stdlib.h>

typedef struct graph 
{
    int numVertex;
    int numEdges;
    int* VI;
    int* EI;
    double* attr;
} graph;

int read_csr (char*, graph*);

void initGraph (graph*, int*);

void freeMem(graph*);
