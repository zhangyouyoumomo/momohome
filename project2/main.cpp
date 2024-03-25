#include <iostream>
#include <vector>
#include <list>
#include <climits>
#include <queue>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

#define MaxTrees 18
#define Capacity 262144 // 2^18 (must be power of 2)
#define Infinity INT_MAX

using namespace std;

struct Edge {
    int to, weight;
    Edge(int t, int w) : to(t), weight(w) {}
};

struct Node {
    int vertex, dist;
    Node(int v, int d) : vertex(v), dist(d) {}
};

// 为std::priority_queue自定义比较器，确保最小元素总是在队列顶部
class CompareDist {
public:
    bool operator()(const Node& n1, const Node& n2) {
        return n1.dist > n2.dist;
    }
};

class BinNode {
public:
  Node Element;
  BinNode* LeftChild;
  BinNode* NextSibling;

  BinNode(Node element) : Element(element), LeftChild(nullptr), NextSibling(nullptr) {}
};

class BinQueue {
public:
  int CurrentSize;  // Total number of nodes
  vector<BinNode*> TheTrees;

  BinQueue() : CurrentSize(0), TheTrees(MaxTrees, nullptr) {}

  ~BinQueue() {  // Destructor to free the allocated nodes
    for (auto tree : TheTrees) {
      delete tree;
    }
  }
  
  BinNode* CombineTrees(BinNode* T1, BinNode* T2) {
    if (T1->Element.dist > T2->Element.dist) {
        swap(T1, T2);
    }
    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    return T1;
  }

  void Merge(BinQueue& H1, BinQueue& H2) {
    BinNode* Carry = nullptr;
    if (H1.CurrentSize + H2.CurrentSize > Capacity) {
      cout << "Out of capacity!" << endl; 
      return; 
    }
    H1.CurrentSize += H2.CurrentSize;
    for (int i = 0, j = 1; j <= H1.CurrentSize; i++, j *= 2) {
      BinNode* T1 = H1.TheTrees[i];
      BinNode* T2 = H2.TheTrees[i];
      int caseNumber = 4*!!Carry + 2*!!T2 + !!T1;
      switch (caseNumber) {
        case 0: // 000
        case 1: // 001
          break;
        case 2: // 010
          H1.TheTrees[i] = T2; H2.TheTrees[i] = nullptr;
          break;
        case 4: // 100
          H1.TheTrees[i] = Carry; Carry = nullptr;
          break;
        case 3: // 011
          Carry = CombineTrees(T1, T2);
          H1.TheTrees[i] = H2.TheTrees[i] = nullptr;
          break;
        case 5: // 101
          Carry = CombineTrees(T1, Carry);
          H1.TheTrees[i] = nullptr;
          break;
        case 6: // 110
          Carry = CombineTrees(T2, Carry);
          H2.TheTrees[i] = nullptr;
          break;
        case 7: // 111
          H1.TheTrees[i] = Carry;
          Carry = CombineTrees(T1, T2);
          H2.TheTrees[i] = nullptr;
          break;
      }
    }
    for (auto& tree : H2.TheTrees) {
      tree = nullptr;
    }
  }
  
  void insert(Node node) {
    BinQueue H0;
    BinNode* newNode = new BinNode(node);
    H0.TheTrees[0] = newNode;
    H0.CurrentSize = 1;
    Merge(*this, H0);
  }

  Node DeleteMin() {
    if (IsEmpty()) {
      cout << "Empty queue!" << endl;
      return Node(-1, -1); // Return an error node
    }

    int MinTree = -1;
    int MinItem = Infinity;
    int MinIndex = -1; // Added to hold the correct index of min element

    // Step 1: Find the smallest item
    for (int i = 0; i < MaxTrees; ++i) {
      if (TheTrees[i] && TheTrees[i]->Element.dist < MinItem) {
        MinItem = TheTrees[i]->Element.dist;
        MinIndex = i; // Keep track of the correct tree
        MinTree = i;
      }
    }

    if (MinTree == -1) return Node(-1, -1); // No minimum found

    // Step 2: Remove the tree containing the minimum item
    BinNode* DeletedTree = TheTrees[MinTree];
    TheTrees[MinTree] = nullptr;

    // Step 3: Create a new tree from the children of the deleted tree
    BinQueue DeletedQueue;
    DeletedQueue.CurrentSize = (1 << MinTree) - 1;
    BinNode* OldRoot = DeletedTree;
    DeletedTree = DeletedTree->LeftChild;
    delete OldRoot; // Free the old root

    // Construct the deleted queue from the children of the minimum item
    for (int j = MinTree - 1; j >= 0 && DeletedTree != nullptr; j--) {
      DeletedQueue.TheTrees[j] = DeletedTree;
      DeletedTree = DeletedTree->NextSibling;
      DeletedQueue.TheTrees[j]->NextSibling = nullptr;
    }

    // Step 4: Merge the original queue with the new queue formed from the deleted tree
    CurrentSize -= DeletedQueue.CurrentSize + 1;
    Merge(*this, DeletedQueue);

    return Node(MinIndex, MinItem);
  }

  bool IsEmpty() const {
    return CurrentSize == 0;
  }
};

using AdjList = vector<list<Edge>>;

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

        if (dist[u] < node.dist) continue; // Check for an outdated queue entry

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
    vector<int> dist(V, Infinity);
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
            if (dist[u] < Infinity && dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.insert(Node(v, dist[v]));
            }
        }
    }
}

bool readGraph(const string& filepath, AdjList& graph) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
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

int main() {
    string graphFile = "USA-road-d.NY.gr"; // Change this path to your graph file
    AdjList graph;

    if (!readGraph(graphFile, graph)) {
        return 1; // If reading fails, terminate the program
    }

    int startNode = 1; // You might want to change this to a valid starting index

    cout << "Using STL priority_queue:" << endl;
    auto start = chrono::high_resolution_clock::now();
    dijkstra_stl(graph, startNode); // Call the STL version of Dijkstra's algorithm
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> stl_duration = end - start;
    cout << "STL Dijkstra Execution Time: " << stl_duration.count() << "ms\n" << endl;

    cout << "Using Binomial Queue:" << endl;
    start = chrono::high_resolution_clock::now();
    dijkstra_binheap(graph, startNode); // Call the Binomial Queue version of Dijkstra's algorithm
    end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> bin_duration = end - start;
    cout << "Binomial Queue Dijkstra Execution Time: " << bin_duration.count() << "ms\n" << endl;

    return 0;
}