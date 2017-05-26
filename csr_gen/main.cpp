#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unordered_map>
#ifndef GRAPH_HEADER_INCL
#include "graph.h"
#include "sort.h"
#endif

#define DEBUG 1

using namespace std;




int main(int argc, char** argv)
{

    // 2 files for 2 different CSR representations
    if (argc != 5)
    {
        printf("Usage : %s <numVertices> <numEdges> <inputFile1> <outputFile>\n", argv[0]);
        exit(1);
    }

    int numVertex = atoi(argv[1]);
    int numEdges = atoi(argv[2]);
    // graph objects
    // G1 -> EI has destination vertices
    // G2 -> EI has source vertices
    int* src = new int [numEdges];
    int* dst = new int [numEdges];

    unsigned int numEdgesRead = 0;

    FILE* fp = fopen (argv[3], "r");
    if (fp == NULL)
    {
        fputs("file error", stderr);
        return -1;
    }

    int id;
//    fscanf(fp, "%d", &id);
//    src[numEdgesRead] = id;
//    fscanf(fp, "%d", &id);
//    dst[numEdgesRead++] = id;


//    char buffer[100];
//    fgets(buffer, 100, fp);
    while(!feof(fp) && (numEdgesRead < numEdges))
    {
        fscanf(fp, "%d", &id);
        src[numEdgesRead] = id;
        fscanf(fp, "%d", &id);
        if (id != src[numEdgesRead])
            dst[numEdgesRead++] = id;
    }
    fclose(fp);
    numEdgesRead--;


    int* inDeg = new int [numVertex]();

    
    for (int i=0; i<numEdgesRead; i++)
        inDeg[src[i]]++;


    graph G1;
    G1.numVertex = numVertex;
    G1.numEdges = numEdgesRead;
    G1.VI = new unsigned int [numVertex]();
    G1.EI = new unsigned int [numEdgesRead]();

    for (int i=1; i<numVertex; i++)
        G1.VI[i] = G1.VI[i-1] + inDeg[i-1];

    for (int i=0; i<numVertex; i++)
        inDeg[i] = 0;

    for (unsigned int i=0; i<numEdgesRead; i++)
    {
        G1.EI[G1.VI[src[i]] + inDeg[src[i]]] = dst[i];
        inDeg[src[i]]++;
    }

    // another csr that stores edges in reverse direction
    // to access children of a node
    write_csr(argv[4], &G1); 


//    write_csr(argv[argc-1], &G2);
    delete[] src;
    delete[] dst;
    delete[] inDeg;
    freeMem(&G1);
//    freeMem(&G2);

    return 0;
}


