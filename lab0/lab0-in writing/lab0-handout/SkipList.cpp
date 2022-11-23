#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include<fstream>

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
        while (q->forwards[i] && q->forwards[i]->key < key) {
            q = q->forwards[i];
        }
    }
    q = q->forwards[0];
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
    /*for (int i = MAX_LEVEL - 1; i >= 0; --i)
    {
        std::cout << "Level " << i + 1 << ":h";
        SKNode *node = head->forwards[i];
        while (node->type != SKNodeType::NIL)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->forwards[i];
        }

        std::cout << "-->N" << std::endl;
    }*/
}

void SkipList::Longsearch(int key1, int key2)
{
    ofstream write;
    LARGE_INTEGER t1, t2, tc;
    QueryPerformanceFrequency(&tc);
    QueryPerformanceCounter(&t1);
    SKNode* q = head;
    int i;
    for (i = MAX_LEVEL - 1; i >= 0; i--) {
        while (q->forwards[i] && q->forwards[i]->key < key1) {
            q = q->forwards[i];
        }
    }
    while (q->forwards[0]->key < key2){
        q = q->forwards[0];
    }
    QueryPerformanceCounter(&t2);
    write.open("skiplist_output.txt", ios::app); 
    write << 1000000 * ((t2.QuadPart - t1.QuadPart) * 1.0 / tc.QuadPart) << endl;
    write.close();
}