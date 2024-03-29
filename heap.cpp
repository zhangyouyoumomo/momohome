// heap.cpp
#include "heap.h"
#include <limits>
#include <algorithm> //swap
#include <cmath>  //for log2

using namespace std;

//FibHeap
FibNode::FibNode(Node element) :Element(element), p(nullptr), left(nullptr), right(nullptr), child(nullptr), degree(0), mark(false) {}
FibHeap::FibHeap() :H_min(nullptr), H_num(0) {}
int FibHeap::IsEmpty() {
	return (this->H_num == 0);
}
void FibHeap::insert(Node node) {
	FibNode* newNode = new FibNode(node);//new return an address
	if (!H_min) {//is first node of fibheap
		H_min = newNode;
		H_min->left = H_min;//referenced to its self
		H_min->right = H_min;
	}
	else {
		newNode->left = H_min->left;
		newNode->right = H_min;//insert before orignal H_min
		H_min->left->right = newNode;/////////////////////////////
		H_min->left = newNode;
		//if (H_min->right == H_min)H_min->right = newNode;//two become circle
		//else H_min->left->right = newNode;
		if (H_min->Element.dist > newNode->Element.dist)//update the minium node
			H_min = newNode;
	}
	this->H_num += 1;
	vertexToaddress[node.vertex] = newNode;//add to map
	return;
}

void FibHeap::link(FibNode* y, FibNode* x) {
	//y一定不是单节点？//至少自己是环
	y->left->right = y->right;//out of rootlist
	y->right->left = y->left;
	//put y into x's childlist
	if (x->child) {//x has child
		y->right = x->child;
		y->left = x->child->left;
		x->child->left->right = y;
		x->child->left = y;
	}
	else {
		y->right = y;
		y->left = y;
	}
	x->child = y;
	x->degree++;
	y->p = x;
	y->mark = false;
}

FibNode* FibHeap::remove_min() {
	FibNode* min = H_min;
	if (H_min == H_min->right) {//only one
		H_min = nullptr;
	}
	else {
		H_min->left->right = H_min->right;//delete in rootlist,not phyisic
		H_min->right->left = H_min->left;
		H_min = H_min->right;
	}
	min->left = min;
	min->right = min;//min修改和H_min修改的是一个东西！
	return min;
}

void FibHeap::CONSOLIDATE() {
	int maxDegree = floor(log2(this->H_num));
	vector<FibNode*>A(maxDegree + 2, nullptr);//index is degree,element is FibNode
	while (H_min) {//if all have from rootlist to A,break
		FibNode* x = remove_min();
		int d = x->degree;
		while (A[d]) {// this degree level has another
			FibNode* y = A[d];
			if (x->Element.dist > y->Element.dist) {//exchange
				FibNode* t = x;
				x = y;
				y = t;
			}
			link(y, x);//link big to small,tem is small,y to x
			A[d] = nullptr;
			d++;//check next if has another
		}
		A[d] = x;//find empty place
	}
	//this->H_min = nullptr;//forget old rootlist
	for (int i = 0; i < maxDegree + 2; i++) {//reconstruct rootlist
		if (A[i]) {
			if (H_min == nullptr) {
				H_min = A[i];
			}
			else {
				A[i]->left = H_min->left;
				A[i]->right = H_min;
				H_min->left->right = A[i];
				H_min->left = A[i];
				if (A[i]->Element.dist < H_min->Element.dist)
					H_min = A[i];//update minium
			}
		}
	}
	return;
}

Node FibHeap::DeleteMin() {
	FibNode* delNode = this->H_min;//the node tobe deleted
	if (delNode) {
		FibNode* Child = delNode->child;
		while (Child) {//put all delnode's children to H's rootlist
			FibNode* tem = Child->right;
			Child->p = nullptr;
			Child->left = delNode->left;//insert before delNode
			Child->right = delNode;
			delNode->left->right = Child;
			delNode->left = Child;
			if (tem->Element.vertex == delNode->child->Element.vertex) {
				break;//finish circle
			}
			Child = tem;
		}
		//remove from root list
		delNode->right->left = delNode->left;
		delNode->left->right = delNode->right;
		if (delNode == delNode->right) {//don't have silbings and children ,empty now
			this->H_min = nullptr;
		}
		else {
			this->H_min = delNode->right;
			this->CONSOLIDATE();//reshape
		}
		this->H_num -= 1;
		Node re(delNode->Element.vertex, delNode->Element.dist);
		delete delNode;//because comes from new
		delNode = NULL;
		return re;
	}
	//empty heap
	return Node(-1, -1);
}

void FibHeap::decrease_key(Node node) {
	int k = node.dist;
	FibNode* x = vertexToaddress[node.vertex];
	x->Element.dist = k;//decrease key
	FibNode* y = x->p;
	if (y && x->Element.dist < y->Element.dist) {//x<y cause problem
		CUT(x, y);
		Cascading_CUT(y);
	}
	if (x->Element.dist < H_min->Element.dist)
		H_min = x;
	return;
}

void FibHeap::CUT(FibNode* x, FibNode* y) {
	if (y->child == x) {//x is first child of y
		if (x == x->right)//sigle child
			y->child = NULL;
		else
			y->child = x->right;
	}
	y->degree--;
	x->left->right = x->right;
	x->right->left = x->left;//cut from old postion
	x->left = H_min->left;
	x->right = H_min;
	H_min->left->right = x;
	this->H_min->left = x;//add x to root list
	x->p = NULL;
	x->mark = false;
	return;
}

void FibHeap::Cascading_CUT(FibNode* y) {
	FibNode* z = y->p;
	if (z) {
		if (y->mark == false)//first cut
			y->mark = true;
		else {//already cut once,forbid keep degenerate
			CUT(y, z);
			Cascading_CUT(z);
		}
	}
	return;
}

//BinQueue
BinNode::BinNode(Node element) : Element(element), LeftChild(nullptr), NextSibling(nullptr) {}

BinQueue::BinQueue() : CurrentSize(0), TheTrees(MaxTrees, nullptr) {}

BinQueue::~BinQueue() {
	for (auto& tree : TheTrees) {
		if (tree != nullptr) {
			delete tree; // Remember to implement a proper recursive delete to avoid memory leaks
			tree = nullptr;
		}
	}
}

BinNode* BinQueue::CombineTrees(BinNode* T1, BinNode* T2) {
	if (T1->Element.dist > T2->Element.dist) {
		std::swap(T1, T2);
	}
	T2->NextSibling = T1->LeftChild;
	T1->LeftChild = T2;
	return T1;
}

void BinQueue::Merge(BinQueue& H1, BinQueue& H2) {
	// Implementation remains the same as provided
	BinNode* Carry = nullptr;
	if (H1.CurrentSize + H2.CurrentSize > Capacity) {
		cout << "Out of capacity!" << endl;
		return;
	}
	H1.CurrentSize += H2.CurrentSize;
	for (int i = 0, j = 1; j <= H1.CurrentSize; i++, j *= 2) {
		BinNode* T1 = H1.TheTrees[i];
		BinNode* T2 = H2.TheTrees[i];
		int caseNumber = 4 * !!Carry + 2 * !!T2 + !!T1;
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

void BinQueue::insert(Node node) {
	BinQueue H0;
	BinNode* newNode = new BinNode(node);
	H0.TheTrees[0] = newNode;
	H0.CurrentSize = 1;
	Merge(*this, H0);
}

Node BinQueue::DeleteMin() {
	// Implementation remains the same as provided, with minor adjustments if needed
	if (IsEmpty()) {
		cout << "Empty queue!" << endl;
		return Node(-1, -1); // Return an error node
	}

	int MinTree = -1;
	int MinItem = INT_MAX;
	int Min_vertex = -1; // Added to hold the correct index of min element

	// Step 1: Find the smallest item
	for (int i = 0; i < MaxTrees; ++i) {
		if (TheTrees[i] && TheTrees[i]->Element.dist < MinItem) {
			MinItem = TheTrees[i]->Element.dist;
			Min_vertex = TheTrees[i]->Element.vertex; // Keep track of the correct tree
			MinTree = i;
		}
	}

	if (MinTree == -1) return Node(-1, -1); // No minimum found

	// Step 2: Remove the tree containing the minimum item
	BinNode* DeletedTree = TheTrees[MinTree];
	TheTrees[MinTree] = nullptr;

	// Step 3: Create a new tree from the children of the deleted tree
	BinQueue DeletedQueue;
	DeletedQueue.CurrentSize = (1 << MinTree) - 1;//because order by size
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

	return Node(Min_vertex, MinItem);
}

bool BinQueue::IsEmpty() const {
	return CurrentSize == 0;
}

// 最小堆比较函数
struct CompareDist {
	bool operator()(const Node& n1, const Node& n2) {
		return n1.dist > n2.dist; // 更小的dist优先
	}
};
//最小堆
//
//
//
//
MinHeap::MinHeap() {}

bool MinHeap::isEmpty() const {
	return heap.empty();
}

void MinHeap::push(Node node) {
	heap.push_back(node);
	int index = heap.size() - 1;
	vertexToIndex[node.vertex] = index;
	heapifyUp(index);
}

void MinHeap::decreaseKey(int vertex, int dist) {
	int index = vertexToIndex[vertex];
	heap[index].dist = dist;
	heapifyUp(index);
}

Node MinHeap::top() {
	return heap.front();
}

void MinHeap::pop() {
	Node lastNode = heap.back();
	heap[0] = lastNode;
	vertexToIndex[lastNode.vertex] = 0;
	heap.pop_back();
	if (!heap.empty()) {
		heapifyDown(0);
	}
	vertexToIndex.erase(lastNode.vertex);
}

void MinHeap::heapifyUp(int index) {
	while (index != 0 && heap[parent(index)].dist > heap[index].dist) {
		std::swap(vertexToIndex[heap[parent(index)].vertex], vertexToIndex[heap[index].vertex]);
		std::swap(heap[index], heap[parent(index)]);
		index = parent(index);
	}
}

void MinHeap::heapifyDown(int index) {
	int smallest = index;
	int leftChildIdx = left(index);
	int rightChildIdx = right(index);

	if (leftChildIdx < heap.size() && heap[leftChildIdx].dist < heap[smallest].dist) {
		smallest = leftChildIdx;
	}

	if (rightChildIdx < heap.size() && heap[rightChildIdx].dist < heap[smallest].dist) {
		smallest = rightChildIdx;
	}

	if (smallest != index) {
		std::swap(vertexToIndex[heap[smallest].vertex], vertexToIndex[heap[index].vertex]);
		std::swap(heap[index], heap[smallest]);
		heapifyDown(smallest);
	}
}

int MinHeap::parent(int index) {
	return (index - 1) / 2;
}

int MinHeap::left(int index) {
	return (2 * index + 1);
}

int MinHeap::right(int index) {
	return (2 * index + 2);
}