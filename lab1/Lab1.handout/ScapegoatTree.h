#include <vector>
#include <iostream>
#include <vector>
using namespace std;

#define ALPHA 0.8

struct STNode
{
    int key;
    STNode* left;
    STNode* right;

    STNode(int k)
        :key(k) {
        left = nullptr;
        right = nullptr;
    }
};

class ScapegoatTree
{
    STNode* root;
    int rebalance_times;
    int size;
    int max_size;
    bool h_balance;
public:
    ScapegoatTree() {
        root = nullptr;
        rebalance_times = 0;
        h_balance = 1;
        size = 0;
        max_size = 0;
    }
    ~ScapegoatTree() {}

    bool IsScapegoat(STNode* p);

    /**
     * @brief Get the Rebalance Times
     *
     * @return int
     */
	int GetRebalanceTimes();

    /**
     * @brief Look up a key in scapegoat tree. Same as trivial binary search tree
     *
     * @param key
     */
    void Search(int key);
    STNode* search(int key);
    /**
     * @brief Insert a new node into the tree. If the key is already in the tree, then do nothing
     *
     * @param key
     */
    void Insert(int key);

    STNode* insert(int key, STNode* &cur, int &depth);

    /**
     * @brief Delete a node of the tree. If the key is not in the tree, do nothing.
     *
     * @param key
     */
    void Delete(int key);

    STNode* Reblance(STNode*& p);

    void PreOrder(STNode* p, vector<int> &arr);

    STNode* BuildTree(vector<int> arr, int l, int r);
    
    void TraceBack(STNode* &cur, int key, STNode* &goat);

    void PreOrder(STNode* p, int& n);
};
