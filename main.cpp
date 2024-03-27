// main.cpp
#include "graph.h"
#include "dijkstra.h"
#include <iostream>
#include <chrono>

int main() {
    std::string graphFile = "USA-road-d.NY.gr";
    AdjList graph;

    if (!readGraph(graphFile, graph)) {
        return 1;
    }

    int startNode = 1;

    std::cout << "Using STL priority_queue:" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    dijkstra_stl(graph, startNode);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> stl_duration = end - start;
    std::cout << "STL Dijkstra Execution Time: " << stl_duration.count() << "ms\n" << std::endl;

    std::cout << "Using Binomial Queue:" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    dijkstra_binheap(graph, startNode);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> bin_duration = end - start;
    std::cout << "Binomial Queue Dijkstra Execution Time: " << bin_duration.count() << "ms\n" << std::endl;

    return 0;
}