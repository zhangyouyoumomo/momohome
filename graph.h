// graph.h
#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <list>
#include <string>

struct Edge {
    int to, weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

using AdjList = std::vector<std::list<Edge>>;//list contains destinations of the node

bool readGraph(const std::string& filepath, AdjList& graph);

#endif // GRAPH_H