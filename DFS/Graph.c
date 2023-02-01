/********************************************************************************* 
* Alfonso Luis Del Rosario, 1698802
* 2023 Winter CSE101 PA2
* Graph.c
*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "Graph.h"
#include "List.h"

#define INF -1
#define NIL -10

#define NULL_ERROR(x) printf("Graph Error: calling %s() on NULL Graph reference\n", x)
#define BOUND_ERROR(x) printf("Graph Error: calling %s() with out of bounds reference\n", x)
#define BFS_ERROR(x) printf("Graph Error: calling %s() without calling BFS beforehand\n", x)
#define EXIT exit(EXIT_FAILURE)

enum color{
    WHITE = 0, BLACK = 1, GRAY = 2
};

typedef struct GraphObj{
    List* list; //double pointer for list of lists.
    int* color;
    int* parent;
    int* distance;
    int* discover; 
    int* finish;
    int sourceVertex;
    int order; //number of vertices 
    int size; //number of edges
}GraphObj;

Graph newGraph(int n)
{
    Graph G = malloc(sizeof(GraphObj));
    assert(G!=NULL);
    G->list = calloc(n, sizeof(List));
    G->color = calloc(n, sizeof(int));
    G->parent = calloc(n, sizeof(int)); 
    G->distance = calloc(n, sizeof(int));
    G->discover = calloc(n, sizeof(int)); 
    G->finish = calloc(n, sizeof(int)); 
    G->sourceVertex = NIL;
    G->order = n;
    G->size = 0;

    for(int i = 0; i < (n); i++)
    {
        G->list[i] = newList();
    }

    for(int i = 0; i < (n); i++)
    {
        G->parent[i] = NIL;
    }

    for(int i = 0; i < (n); i++)
    {
        G->distance[i] = INF;
    }
    
    for(int i = 0; i < (n); i++)
    {
        G->discover[i] = INF; //CHANGE
    }
    
    for(int i = 0; i < (n); i++)
    {
        G->finish[i] = INF; //CHANGE
    }

    return G;
}

void freeGraph(Graph* pG)
{
    if(*pG == NULL)
    {
        NULL_ERROR("freeGraph");
        EXIT;   
    }
    
    if(pG != NULL && *pG != NULL)
    {
        for(int i = 0; i < (*pG)->order; i++)
        {
            freeList(&((*pG)->list[i]));
        }
        free((*pG)->list);
        free((*pG)->distance);
        free((*pG)->parent);
        free((*pG)->color);
        free((*pG)->discover);
        free((*pG)->finish);
        free(*pG);
        *pG = NULL;
    }
}

/*** Access functions ***/

int getOrder(Graph G)
{
    if(G == NULL)
    {
        NULL_ERROR("getOrder");
        EXIT;
    }

    return G->order;
}

int getSize(Graph G)
{
    if(G == NULL)
    {
        NULL_ERROR("getSize");
        EXIT;
    }

    return G->size; 
}

int getSource(Graph G)
{
    if(G == NULL)
    {
        NULL_ERROR("getSource");
        EXIT;
    }

    return G->sourceVertex;
}

int getParent(Graph G, int u)
{
    if(G == NULL)
    {
        NULL_ERROR("getDist");
        EXIT;
    }

    if(u < 1 || u > getOrder(G))
    {
        BOUND_ERROR("getParent");
        EXIT;
    }

    if(G->sourceVertex == NIL)
    {
        return NIL;
    }

    return G->parent[u-1];
}

int getDiscover(Graph G, int u)
{
    if(G == NULL)
    {
        NULL_ERROR("getDiscover");
        EXIT;
    }

    if(u < 1 || u > getOrder(G))
    {
        BOUND_ERROR("getDiscover");
        EXIT;
    }

    //CHANGE: check preconditions

    return G->discover[u-1];
}

int getFinish(Graph G, int u)
{
    if(G == NULL)
    {
        NULL_ERROR("getFinish");
        EXIT;
    }

    if(u <  1 || u  > getOrder(G))
    {
        BOUND_ERROR("getFinish");
        EXIT;
    }

    //CHANGE: check preconditions

    return G->finish[u-1];
}

Graph transpose(Graph G)
{
    Graph t_graph = newGraph(getOrder(G));

    return t_graph;
}

Graph copyGraph(Graph G)
{
    Graph copy_graph = newGraph(getOrder(G));

    

    return copy_graph;
}

int getDist(Graph G, int u)
{
    if(G == NULL)
    {
        NULL_ERROR("getDist");
        EXIT;
    }

    if(u < 1 || u > getOrder(G))
    {
        BOUND_ERROR("getDist");
        EXIT;
    }

    if(G->sourceVertex == NIL)
    {
        return INF;
    }
    
    return G->distance[u-1];
}

void getPath(List L, Graph G, int u)
{
    if(G == NULL || L == NULL)
    {
        NULL_ERROR("getPath");
        EXIT;
    }

    if(u < 1 || u > getOrder(G))
    {
        BOUND_ERROR("getPath");
        EXIT;
    }

    if(G->sourceVertex == NIL)
    {
        BFS_ERROR("getPath");
        EXIT;
    } 
    
    if(u != getSource(G) && getParent(G, u) == NIL)
    {
        append(L, NIL);
        return;
    }

    List temp = newList();

    int current = u;
    while(current != NIL)
    {
        prepend(temp, current);
        current = getParent(G, current);
    }

    int i = 0;
    moveFront(temp);
    while(i < length(temp))
    {
        append(L, get(temp));
        moveNext(temp);
        i++;
    }

    freeList(&temp);
}

/*** Manipulation procedures ***/

void makeNull(Graph G)
{
    if (G == NULL)
    {
        NULL_ERROR("makeNull");
        EXIT;
    }

    int i = 0;

    while(i < getOrder(G))
    {
        clear(G->list[i]); 
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->distance[i] = INF;
        i++;
    }

    G->size = 0;
    G->sourceVertex = NIL;
}

void addEdge(Graph G, int u, int v) 
{
    if(G==NULL)
    {
        NULL_ERROR("addEdge");
        EXIT;
    }

    if(u < 1 || u > getOrder(G))
    {
        BOUND_ERROR("addEdge");
        EXIT;
    }

    if(v < 1 || v > getOrder(G))
    {
        BOUND_ERROR("addEdge");
        EXIT;
    }

    List U = G->list[u-1];
    List V = G->list[v-1];

    if(length(U) == 0)
    {
        append(U, v);
    }
    else
    {
        if(v > back(U))
        {
            append(U, v);
        }
        else
        {
            moveFront(U);
            while(v > get(U))
            {
                moveNext(U);
            }
            insertBefore(U,v);
        }
    }

    if(length(V) == 0)
    {
        append(V, u);
    }
    else
    {
        if(u > back(V))
        {
            append(V, u);
        }
        else
        {
            moveFront(V);
            while(u > get(V))
            {                
                moveNext(V);
            }
            insertBefore(V,u);
        }
    }
    G->size++;
}

void addArc(Graph G, int u, int v)
{
    if(G==NULL)
    {
        NULL_ERROR("addArc");
        EXIT;
    }

    if(u < 1 || u > getOrder(G))
    {
        BOUND_ERROR("addArc");
        EXIT;
    }

    if(v < 1 || v > getOrder(G))
    {
        BOUND_ERROR("addArc");
        EXIT;
    }

    List U = G->list[u-1];

    if(length(U) == 0)
    {
        append(U, v);
    }
    else
    {
        if(u > back(U))
        {
            append(U, v);
        }
        else
        {
            moveFront(U);
            while(u > get(U))
                moveNext(U);
            insertBefore(U, v);
        }
    }
    G->size++;
}

void Visit(Graph G, int x, int time)
{
    G->distance[x] = ++time;
    G->color[x] = GRAY;
    

}

void DFS(Graph G, List S)
{
    if(G == NULL)
    {
        NULL_ERROR("DFS");
        EXIT;
    }   

    for(int i = 0; i < getOrder(G); i++)
    {
        G->color[i] = WHITE;
        G->parent[i] = NIL;
    }

    static int time = 0;

    for(int i = 0; i < getOrder(G); i++)
    {
        if(G->color[i] == WHITE)
        {
            Visit(G, i, time);
        }
    }
}

void BFS(Graph G, int s)
{
    if(G == NULL)
    {
        NULL_ERROR("BFS");
        EXIT;
    }

    G->sourceVertex = s;

    //sets all elements to undiscovered
    for(int i = 0; i < getOrder(G); i++)
    {
        G->color[i] = WHITE;
        G->distance[i] = INF;
        G->parent[i] = NIL;
    }

    //sets source vertex to discovered
    G->distance[s-1] = 0;
    G->color[s-1] = GRAY;
    G->parent[s-1] = NIL;

    int curr = 0;

    List L = newList();
    prepend(L, s);
    while(length(L) != 0)
    {
        moveFront(L);
        curr = get(L)-1;
        deleteFront(L);
        List temp = G->list[curr];
        moveFront(temp);
        while(index(temp) != -1)
        {
            int adj_index = get(temp)-1;
            if(G->color[adj_index] == WHITE)
            {
                G->color[adj_index] = GRAY;
                G->distance[adj_index] = G->distance[curr]+1;
                G->parent[adj_index] = curr+1;
                append(L, get(temp));
            }
            moveNext(temp);
        }
        G->color[curr] = BLACK;
    }
    freeList(&L);
}

/*** Other operations ***/
void printGraph(FILE* out, Graph G)
{
    if(G == NULL)
    {
        NULL_ERROR("printGraph");
        EXIT;
    }
    
    for(int i = 0; i < G->order; i++)
    {
        fprintf(out, "%d: ", i+1);
        printList(out, G->list[i]);
        fprintf(out, "\n");
    }
}