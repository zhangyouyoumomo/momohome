// heap.h
#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <iostream>
#include <unordered_map>

#define MaxTrees 18
#define Capacity 262144 // Example capacity, adjust as necessary
#define Infinity std::numeric_limits<int>::max()

using namespace std;
struct Node {
	int vertex, dist;
	Node(int v = -1, int d = Infinity) : vertex(v), dist(d) {} // Default constructor adjusted for simplicity
};

//FibHeap
class FibNode {
public:
	Node Element;
	FibNode* p;
	FibNode* child;
	FibNode* left;
	FibNode* right;
	int degree;
	bool mark;
	FibNode(Node element);
};

class FibHeap {
public:
	FibNode* H_min;
	int H_num;
	FibHeap();
	void insert(Node node);
	Node DeleteMin();
	void CONSOLIDATE();
	void link(FibNode* y, FibNode* x);
	void decrease_key(Node node);
	void CUT(FibNode* x, FibNode* y);
	void Cascading_CUT(FibNode* y);
	int IsEmpty();
	std::unordered_map<int, FibNode*> vertexToaddress; // 顶点到堆中索引的映射
};

//BinQueue
class BinNode {
public:
	Node Element;
	BinNode* LeftChild;
	BinNode* NextSibling;

	BinNode(Node element);
};

class BinQueue {
public:
	int CurrentSize;  // Total number of nodes
	vector<BinNode*> TheTrees;

	BinQueue();
	~BinQueue();

	void Merge(BinQueue& H1, BinQueue& H2);
	void insert(Node node);
	Node DeleteMin();
	bool IsEmpty() const;

private:
	BinNode* CombineTrees(BinNode* T1, BinNode* T2);
};

//MinHeap
class MinHeap {
private:
	std::vector<Node> heap; // 堆
	std::unordered_map<int, int> vertexToIndex; // 顶点到堆中索引的映射

	void heapifyUp(int index);
	void heapifyDown(int index);
	int parent(int index);
	int left(int index);
	int right(int index);

public:
	MinHeap();
	bool isEmpty() const;
	void push(Node node);
	void decreaseKey(int vertex, int dist);
	Node top();
	void pop();
};

#endif // HEAP_H