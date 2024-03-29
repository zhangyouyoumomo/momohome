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
	MinHeap pq;

	dist[src] = 0;
	pq.push(Node(src, 0));

	while (!pq.isEmpty()) {
		Node node = pq.top();
		pq.pop();
		int u = node.vertex;

		for (const auto& i : graph[u]) {
			int v = i.to;
			int weight = i.weight;
			if (dist[v] > dist[u] + weight) {
				if (dist[v] == INT_MAX) {
					dist[v] = dist[u] + weight;
					pq.push(Node(v, dist[v]));
				}
				else {
					dist[v] = dist[u] + weight;
					pq.decreaseKey(v, dist[v]);
				}
			}
		}
		/*
		cout << node.vertex << endl;
		for (const auto& j : dist) {
			if (j > 10)cout << "#";
			else
				cout << j;
		}
		cout << endl;
		*/
	}

	for (int i = 0; i < V; ++i)
		if (dist[i] != Infinity)
			cout << "Node " << i << ", Min Distance: " << dist[i] << endl;
		else
			cout << "Node " << i << ", Unreachable" << endl;
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

	for (int i = 0; i < V; ++i)
		if (dist[i] != Infinity)
			cout << "Node " << i << ", Min Distance: " << dist[i] << endl;
		else
			cout << "Node " << i << ", Unreachable" << endl;
}

void dijkstra_fibheap(const AdjList& graph, int src) {
	int V = graph.size();
	vector<int> dist(V, INT_MAX);
	FibHeap pq;
	dist[src] = 0;
	pq.insert(Node(src, 0));

	while (!pq.IsEmpty()) {
		Node u_node = pq.DeleteMin();
		int u = u_node.vertex;
		if (u == -1) break; // Check for an error node

		//if (dist[u] < u_node.dist) continue;  // Check for an outdated queue entry

		for (const auto& i : graph[u]) {
			int v = i.to;
			int weight = i.weight;
			if (dist[v] > dist[u] + weight) {
				if (dist[v] == INT_MAX) {
					dist[v] = dist[u] + weight;
					pq.insert(Node(v, dist[v]));
				}
				else {
					dist[v] = dist[u] + weight;
					pq.decrease_key(Node(v, dist[v]));
				}
			}
		}
		/*
		cout << u_node.vertex << endl;
		for (const auto& j : dist) {
			if (j > 10)cout << "#";
			else
				cout << j;
		}
		cout << endl;
		*/
	}

	for (int i = 0; i < V; ++i)
		if (dist[i] != Infinity)
			cout << "Node " << i << ", Min Distance: " << dist[i] << endl;
		else
			cout << "Node " << i << ", Unreachable" << endl;
}