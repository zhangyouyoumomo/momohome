// heap.h
#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <iostream>

#define MaxTrees 18
#define Capacity 262144 // Example capacity, adjust as necessary
#define Infinity std::numeric_limits<int>::max()

struct Node {
	int vertex, dist;
	Node(int v = -1, int d = Infinity) : vertex(v), dist(d) {} // Default constructor adjusted for simplicity
};

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
	std::vector<BinNode*> TheTrees;

	BinQueue();
	~BinQueue();

	void Merge(BinQueue& H1, BinQueue& H2);
	void insert(Node node);
	Node DeleteMin();
	bool IsEmpty() const;

private:
	BinNode* CombineTrees(BinNode* T1, BinNode* T2);
};

#endif // HEAP_H