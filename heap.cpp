// heap.cpp
#include "heap.h"
#include <limits>
#include <algorithm> // For std::swap

using namespace std;

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
	// Implementation remains the same as provided
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

bool BinQueue::IsEmpty() const {
	return CurrentSize == 0;
}