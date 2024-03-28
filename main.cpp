// main.cpp
#include "graph.h"
#include "dijkstra.h"
#include <iostream>
#include <chrono>

int main() {
	//std::string graphFile = "USA-road-d.NY.gr";
	std::string graphFile = "1.txt";
	AdjList graph;

	if (!readGraph(graphFile, graph)) {
		return 1;
	}

	int startNode = 0;

	std::cout << "Using Min Heap:" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	dijkstra_stl(graph, startNode);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> stl_duration = end - start;
	std::cout << "Min Heap Dijkstra Execution Time: " << stl_duration.count() << "ms\n" << std::endl;

	std::cout << "Using Binomial Queue:" << std::endl;
	start = std::chrono::high_resolution_clock::now();
	dijkstra_binheap(graph, startNode);
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> bin_duration = end - start;
	std::cout << "Binomial Queue Dijkstra Execution Time: " << bin_duration.count() << "ms\n" << std::endl;

	std::cout << "Using Fib Heap:" << std::endl;
	start = std::chrono::high_resolution_clock::now();
	dijkstra_fibheap(graph, startNode);
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> fib_duration = end - start;
	std::cout << "Fib Heap Dijkstra Execution Time: " << bin_duration.count() << "ms\n" << std::endl;

	return 0;
}