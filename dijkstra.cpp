#include <vector>
#include <list>
#include <queue>
#include <climits>
#include "dijkstra.h"



using namespace std;

// Custom comparator for priority queue
class CompareDist {
public:
    bool operator()(const Node& n1, const Node& n2) {
        return n1.dist > n2.dist;
    }
};

void dijkstra_stl(const AdjList& graph, int src) {
    int V = graph.size();
    vector<int> dist(V, INT_MAX);
    priority_queue<Node, vector<Node>, CompareDist> pq;

    dist[src] = 0;
    pq.push(Node(src, 0));

    while (!pq.empty()) {
        Node node = pq.top();
        pq.pop();
        int u = node.vertex;

        if (dist[u] < node.dist) continue;

        for (const auto& i : graph[u]) {
            int v = i.to;
            int weight = i.weight;
            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push(Node(v, dist[v]));
            }
        }
    }
}

void dijkstra_binheap(const AdjList& graph, int src) {
    int V = graph.size();
    vector<int> dist(V, INT_MAX);
    BinQueue pq;
  
    dist[src] = 0;
    pq.insert(Node(src, 0));

    while (!pq.IsEmpty()) {
        Node u_node = pq.DeleteMin();
        int u = u_node.vertex;
        if (u == -1) break; // Check for an error node

        if (dist[u] < u_node.dist) continue;  // Check for an outdated queue entry

        for (const auto& i : graph[u]) {
            int v = i.to;
            int weight = i.weight;
            if (dist[u] < INT_MAX && dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.insert(Node(v, dist[v]));
            }
        }
    }
}