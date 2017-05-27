#include "graph.h"

int read_csr (char* filename, graph* G)
{
    FILE* graphFile = fopen(filename, "rb");
    if (graphFile == NULL)
    {
        fputs("file error", stderr);
        return -1;
    }
    fread (&(G->numVertex), sizeof(int), 1, graphFile);
    fread (&(G->numEdges), sizeof(int), 1, graphFile);

    G->VI = new int[G->numVertex+1];
    fread (G->VI, sizeof(int), G->numVertex, graphFile);
    G->VI[G->numVertex] = G->numEdges;
    if (feof(graphFile))
    {
        delete[] G->VI;
        printf("unexpected end of file while reading vertices\n");
        return -1;
    }
    else if (ferror(graphFile))
    {
        delete[] G->VI;
        printf("error reading file\n");
        return -1;
    }

    G->EI = new int[G->numEdges];
    fread (G->EI, sizeof(int), G->numEdges, graphFile);
    if (feof(graphFile))
    {
        delete[] G->EI;
        delete[] G->VI;
        printf("unexpected end of file while reading edges\n");
        return -1;
    }
    else if (ferror(graphFile))
    {
        delete[] G->EI;
        delete[] G->VI;
        printf("error reading file\n");
        return -1;
    }
    fclose(graphFile);

    return 1;
}

void initGraph (graph* G, int* outDeg)
{
    G->attr = new double [G->numVertex];
    int i = 0;
    for (i=0; i<G->numVertex; i++)
        G->attr[i]   = (1.0); 
    return;
}

void freeMem (graph* G)
{
    delete[] G->VI;
    delete[] G->EI;
    delete[] G->attr;
}
