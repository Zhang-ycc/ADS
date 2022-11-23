#include <iostream>
#include <stdlib.h>

#include "SkipList.h"

double SkipList::my_rand()
{
    s = (16807 * s) % 2147483647ULL;
    return (s + 0.0) / 2147483647ULL;
}

int SkipList::randomLevel()
{
    int result = 1;
    while (result < MAX_LEVEL && my_rand() < 0.5)
    {
        ++result;
    }
    return result;
}

void SkipList::Insert(int key, int value)
{
    // TODO
    SKNode* q = head;
    int i;
    for (i = MAX_LEVEL - 1; i >= 0; i--) {
        while (q->forwards[i] && q->forwards[i]->key < key) {
            q = q->forwards[i];
        }
    }
    q = q->forwards[0];
    if (q->key == key) {
        q->val = value;
        return;
    }
    int level = randomLevel();
    q = head;
    SKNode* new_node = new SKNode(key, value, NORMAL);
    for (int i = level-1; i >= 0; i--) {
        while (q->forwards[i] && q->forwards[i]->key < key)
            q = q->forwards[i];
        new_node->forwards[i] = q->forwards[i];
        q->forwards[i] = new_node;
    }
}

void SkipList::Search(int key)
{
    // TODO
    SKNode* q = head;
    int i;
    for (i = MAX_LEVEL-1; i >= 0; i--) {
        cout << (i+1) << ',';
        if (q->type == 1)
            cout << "h ";
        else if (q->type == 3)
            cout << "N ";
        else
            cout << q->key << ' ';
        while (q->forwards[i] && q->forwards[i]->key < key) {
            q = q->forwards[i];
            cout << (i + 1) << ',';
            if (q->type == 1)
                cout << "h ";
            else if (q->type == 3)
                cout << "N ";
            else
                cout << q->key << ' ';
        }
    }
    q = q->forwards[0];
    cout << (i + 2) << ',';
    if (q->type == 1)
        cout << "h ";
    else if (q->type == 3)
        cout << "N ";
    else
        cout << q->key << ' ';
    if (q->key == key)
        cout << q->val << endl;
    else
        cout << "Not Found" << endl;

}

void SkipList::Delete(int key)
{
    SKNode* q = head;
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (q->forwards[i] && q->forwards[i]->key < key) {
            q = q->forwards[i];
        }
        if (q->forwards[i]->key == key)
            q->forwards[i] = (q->forwards[i])->forwards[i];
    }
}

void SkipList::Display()
{
    for (int i = MAX_LEVEL - 1; i >= 0; --i)
    {
        std::cout << "Level " << i + 1 << ":h";
        SKNode *node = head->forwards[i];
        while (node->type != SKNodeType::NIL)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->forwards[i];
        }

        std::cout << "-->N" << std::endl;
    }
}