#include <iostream>

#include "HashTable.h"

int HashTable::hash(int key)
{
    return key % BUCKET_SIZE;
}

void HashTable::Insert(int key, int value)
{
    int k = hash(key);
    if (!bucket[k]) {
        bucket[k] = new HashNode(key,value);
    }
    else {
        HashNode* p = bucket[k];
        bool flag = 0;
        if (p->key == key) {
            flag = 1;
            p->val = value;
        }
        else {
            while (p->next) {
                p = p->next;
                if (p->key == key) {
                    flag = 1;
                    p->val = value;
                }
            }
        }
        if (!flag) {
            HashNode* q = new HashNode(key, value);
            p->next = q;
        }
    }
    // TODO
}

void HashTable::Search(int key)
{
    int k = hash(key);
    bool flag = 0;
    int idex = -1;
    HashNode* p = bucket[k];
    if (bucket[k]) {
        while (p) {
            idex++;
            if (p->key == key) {
                flag = 1;
                break;
            }
            p = p->next;
        }
    }
    if (!flag)
        cout << "Not Found\n";
    else
        cout << "bucket " << k << " index " << idex <<" key "<< p->key <<" value " << p->val <<endl;
    // TODO
}

void HashTable::Delete(int key)
{
    int k = hash(key);
    if (bucket[k]) {
        if (bucket[k]->key == key) {
            HashNode* q = bucket[k]->next;
            if (q) {
                bucket[k] = new HashNode(q->key, q->val, q->next);
            }
            else
                bucket[k] = nullptr;
        }
        else {
            HashNode* p = bucket[k];
            while (p->next) {
                HashNode* q = p->next;
                if (q->key == key) {
                    p->next = q->next;
                    delete q;
                    break;
                }
                p = p->next;
            }
        }
    }
    // TODO
}

void HashTable::Display()
{
    for (int i = 0; i < BUCKET_SIZE; ++i)
    {
        std::cout << "|Bucket " << i << "|";
        HashNode* node = bucket[i];
        while (node)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->next;
        }
        std::cout << "-->" << std::endl;
    }
}