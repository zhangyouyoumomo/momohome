// dijkstra.h
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"//get struct of graph
#include "heap.h" // Include the heap definitions

void dijkstra_stl(const AdjList& graph, int src);
void dijkstra_binheap(const AdjList& graph, int src);
void dijkstra_fibheap(const AdjList& graph, int src);

#endif // DIJKSTRA_H