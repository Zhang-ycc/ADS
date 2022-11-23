#include "ScapegoatTree.h"
#include <iostream>
#include <cmath>
#include <algorithm>

int ScapegoatTree::GetRebalanceTimes()
{
	return rebalance_times;
}

void ScapegoatTree::Search(int key)
{
	//printf("Search %d\n", key);
	STNode* cur = root;
	int depth = 0;
	while (cur) {
		if (key < cur->key) {
			cur = cur->left;
			depth++;
		}
		else if (key > cur->key) {
			cur = cur->right;
			depth++;
		}
		else {
			//cout << depth << endl;
			return;
		}
	}
	//cout << "Not Found\n";
}

STNode* ScapegoatTree::search(int key)
{
	STNode* cur = root;
	while (cur) {
		if (key < cur->key) {
			cur = cur->left;
		}
		else if (key > cur->key) {
			cur = cur->right;
		}
		else {
			return cur;
		}
	}
	return nullptr;
}

void ScapegoatTree::Insert(int key)
{
	//printf("Insert %d\n", key);
	if (search(key) == nullptr)
		size++;
	//printf("size:%d ", size);
	int depth = 0;
	root = insert(key, root, depth);
	if (size > max_size)
		max_size = size;
	//printf("key:%d,depth:%d  %d, %d\n", key, depth, size, max_size);
}

STNode* ScapegoatTree::insert(int key, STNode* &cur, int& depth)
{
	if (cur == nullptr) {
		cur = new STNode(key);
		//printf("key:%d,depth:%d\n", key, depth);
		if (depth > log(size) / log(1 / ALPHA)) {
			h_balance = 0;
		}
		return cur;
	}
	else if (cur->key == key) {
		if (depth > log(size) / log(1 / ALPHA)) {
			h_balance = 0;
		}
		return cur;
	}
	//printf("%d,depth:%d\n", cur->key, depth);
	depth++;
	if (key < cur->key) {
		cur->left = insert(key, cur->left, depth);
	}
	else if (key > cur->key) {
		cur->right = insert(key, cur->right, depth);
	}
	if (!h_balance && IsScapegoat(cur)) {
		h_balance = 1;
		cur = Reblance(cur);
		rebalance_times++;
		//printf("goat: %d\n", cur->key);
	}
	return cur;
}

void ScapegoatTree::TraceBack(STNode* &cur, int key, STNode*& goat)
{
	if (key < cur->key) {
		TraceBack(cur->left, key, goat);
	}
	else if (key > cur->key) {
		TraceBack(cur->right, key, goat);
	}
	else
		return;
	
	return;
}

void ScapegoatTree::Delete(int key)
{
	/*if (key == 38) {
		printf(":%d\n", root->left->right->key);
	}*/
	//printf("Delete %d\n", key);
	STNode *cur = root, *prev;
	while (cur && cur->key!=key)
	{
		if (key < cur->key) {
			prev = cur;
			cur = cur->left;
		}
		else if (key > cur->key) {
			prev = cur;
			cur = cur->right;
		}
	}
	if (cur == nullptr) {
		if (size <= ALPHA * max_size) {
			root = Reblance(root);
			max_size = size;
			rebalance_times++;
		}
		return;
	}
	if (cur->right == nullptr) {
		if (cur == root) {
			root = cur->left;
		}
		else if (cur == prev->left) {
			prev->left = cur->left;
		}
		else if (cur == prev->right) {
			prev->right = cur->left;
		}
		delete cur;
	}
	else if (cur->left == nullptr) {
		if (cur == root) {
			root = cur->right;
		}
		else if (cur == prev->left) {
			prev->left = cur->right;
		}
		else if (cur == prev->right) {
			prev->right = cur->right;
		}
		delete cur;
	}
	else
	{
		STNode* p = cur->right, *pre = cur;
		while (p->left)
		{
			pre = p;
			p = p->left;
		}
		cur->key = p->key;
		if (pre->left == p) {
			pre->left = p->right;
		}
		else {
			pre->right = p->right;
		}
		delete p;
		//if (key == 6)
		//printf(":%d", cur->right->key);
	}
	if (--size <= ALPHA * max_size) {
		root = Reblance(root);
		//printf("Delete re %d, %d, %d\n", key, size, max_size);
		//printf("  %d\n", root->right->key);
		max_size = size;
		rebalance_times++;
	}
}

bool ScapegoatTree::IsScapegoat(STNode* p)
{
	int ls = 0, rs = 0;
	PreOrder(p->left, ls);
	PreOrder(p->right, rs);
	int total = ls + rs + 1;
	double ratio = std::max(ls, rs) / double(total);
	//printf("key:%d (%d,%d,%f) \n", p->key, ls, rs,ratio);
	return (ratio > ALPHA);
}

STNode* ScapegoatTree::Reblance(STNode*& p)
{
	vector<int> arr;
	PreOrder(p, arr);
	//for (int i = 0; i < arr.size(); i++)
	//	printf("%d ", arr[i]);
	//printf("\n");
	return BuildTree(arr, 0, arr.size() -1);
}

void ScapegoatTree::PreOrder(STNode* p, vector<int> &arr)
{
	if (p) {
		PreOrder(p->left, arr);
		arr.push_back(p->key);
		//printf("%d ", p->key);
		PreOrder(p->right, arr);
	}
	return;
}

STNode* ScapegoatTree::BuildTree(vector<int> arr, int l, int r)
{
	if (l > r)
		return nullptr;
	int m = (l + r) / 2;
	STNode* new_root = new STNode(arr[m]);
	//printf("p key:%d (%d,%d) \n", new_root->key, l, r);
	new_root->left = BuildTree(arr, l, m - 1);
	new_root->right = BuildTree(arr, m + 1, r);
	//printf("o key:%d (%d,%d) \n", new_root->key, l, r);
	return new_root;
}

void ScapegoatTree::PreOrder(STNode* p, int &n)
{
	if (p) {
		n++;
		PreOrder(p->left, n);
		PreOrder(p->right, n);
	}
	return;
}