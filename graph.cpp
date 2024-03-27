#include "graph.h"
#include <fstream>
#include <sstream>
#include<iostream>



using namespace std;

bool readGraph(const string& filepath, AdjList& graph) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return false;
    }

    string line;
    int numVertices = 0, numEdges = 0;

    while (getline(file, line)) {
        if (line[0] == 'c') continue; // Comment line, skip
        else if (line[0] == 'p') {
            istringstream iss(line);
            string tmp;
            iss >> tmp >> tmp >> numVertices >> numEdges;
            graph.resize(numVertices + 1); // Since graphs in .gr files are 1-indexed
        } else if (line[0] == 'a') {
            int from, to, weight;
            istringstream iss(line);
            char arc;
            iss >> arc >> from >> to >> weight;
            graph[from].push_back(Edge(to, weight)); // Assuming directed graph
        }
    }

    file.close();
    return true;
}