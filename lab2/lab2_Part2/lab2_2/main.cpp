#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <string.h>
#include <windows.h>
#include <stack>
#include <thread>
#include <vector>
#include<algorithm>

using namespace std;

#define INF 1e6

struct Dijkstra
{
    int dist;
    vector<int> path;
    Dijkstra(int d, vector<int> p) {
        dist = d;
        path = p;
    }
};

int n;
int N;
int _d;
int M;
vector<vector<int>> matrix;
vector<vector<Dijkstra>> min_distance;
vector<int> intermediates;

void slaveMinDistance(int id);
vector<int> getFixedPointShortestPath(int source);
Dijkstra getMinDistance(int src, int dest);
void perm(vector<int> list, int low, int high, vector<vector<int>>& c);
void swap(int& a, int& b);


void do_test(string input_file_path)
{
    ifstream inputData;
    inputData.open(input_file_path, ios::in);
    if (!inputData)
    {
        cout << "[error]: file " << input_file_path << " not found." << endl;
        inputData.close();
        return;
    }

    string str;
    inputData >> str;
    int node_num = atoi(str.c_str());

    matrix.resize(node_num, vector<int>(node_num));

    for(int i = 0; i < node_num; ++i){
        for(int j = 0; j < node_num; ++j){
            inputData >> str;
            matrix[i][j] = str == "@" ? INF : atoi(str.c_str());
        }
    }

    n = matrix.size();
    Dijkstra p(0, vector<int>(1, 0));
    min_distance.resize(1000, vector<Dijkstra>(1000, p));

    

    while (inputData >> str)
    {
        int source = atoi(str.c_str());
        while(true){
            inputData >> str;
            if(str == "$"){
                break;
            }
            intermediates.emplace_back(atoi(str.c_str()));
        }

        LARGE_INTEGER t1, t2, tc;
        QueryPerformanceFrequency(&tc);
        QueryPerformanceCounter(&t1);

        vector<int> path = getFixedPointShortestPath(source);

        QueryPerformanceCounter(&t2);
        double time = 1000 * ((t2.QuadPart - t1.QuadPart) * 1.0 / tc.QuadPart);
        cout << "M = " << M << " " << time << endl;

        int dis = 0;
        for(int i = 0; i < path.size(); ++i){
            dis += matrix[path[i]][path[(i + 1) % path.size()]];
        }
        if(dis == 0){
            dis = INF;
        }
        //cout << dis << endl;

        intermediates.clear();
        min_distance.clear();
        Dijkstra p(0, vector<int>(1, 0));
        min_distance.resize(1000, vector<Dijkstra>(1000, p));
    }
    
    inputData.close();
}


int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        cout << "[usage]: ./main options [input_file_path]" << endl;
        return 0;
    }

    switch (*argv[1])
    {
    case 'd':
        for (M = 1; M <= 8; M *= 2) {
            do_test(argv[2]);
        }
        break;
    default:
        cout << "[error]: option not support, use d(debug), or g(grade)." << endl;
    }

    return 0;
}

void slaveMinDistance(int id) {
    int start = id * _d;
    int end = start + _d;
    for (int i = start; i < end && i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                min_distance[intermediates[i]][intermediates[j]] = getMinDistance(intermediates[i], intermediates[j]);
            }
        }
    }
    //cout << "线程" << id << "完成\n";
}

vector<int> getFixedPointShortestPath(int source)
{
    vector<int> path;
    
    intermediates.push_back(source);
    N = intermediates.size();
    _d = (N % M == 0) ? N / M : (N / M + 1);

    vector<thread> threads;
    for (int i = 0; i < M; i++) {
        threads.emplace_back(slaveMinDistance, i);
    }

    for (int i = 0; i < M; i++)
        threads[i].join();

    /*for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                min_distance[intermediates[i]][intermediates[j]] = getMinDistance(intermediates[i], intermediates[j]);
            }
        }
    }*/

    intermediates.pop_back();

    vector<vector<int>> c;
    perm(intermediates, 0, intermediates.size() - 1, c);

    int min = INT32_MAX;
    vector<int> min_c;
    for (auto cur : c) {

        int tmp = min_distance[source][cur[0]].dist + min_distance[source][cur[cur.size() - 1]].dist;
        for (int i = 0; i < cur.size() - 1; i++) {
            tmp += min_distance[cur[i]][cur[i + 1]].dist;
        }
        if (tmp < min) {
            min = tmp;
            min_c = cur;
        }
    }

    if (min >= INF)
        return path;

    Dijkstra t = min_distance[source][min_c[0]];
    for (auto cur : t.path) {
        path.push_back(cur);
    }
    for (int i = 0; i < min_c.size() - 1; i++) {
        t = min_distance[min_c[i]][min_c[i + 1]];
        for (auto cur : t.path) {
            path.push_back(cur);
        }
    }
    t = min_distance[min_c[min_c.size() - 1]][source];
    for (auto cur : t.path) {
        path.push_back(cur);
    }
    path.push_back(source);

    return path;
}

Dijkstra getMinDistance(int src, int dest)
{
    vector<bool> flag(n, false);
    vector<int> dist(n, INT32_MAX);
    vector<int> path(n, -1); 

    for (int i = 0; i < n; i++)
    {
        if (matrix[src][i] != INF)
        {
            dist[i] = matrix[src][i];
            path[i] = src;
        }
    }
    dist[src] = 0;
    flag[src] = true;

    int min_prev = -1;
    int min_dis = INT32_MAX;
    while (1)
    {
        min_prev = -1;
        min_dis = INT32_MAX;
        for (int i = 0; i < n; i++)
        {
            if (!flag[i])
            {
                if (dist[i] < min_dis)
                {
                    min_dis = dist[i];
                    min_prev = i;
                }
            }
        }
        if (min_prev == -1)
            break;

        for (int i = 0; i < n; i++)
        {
            if (matrix[min_prev][i] != INF)
            {
                if (dist[min_prev] + matrix[min_prev][i] < dist[i])
                {
                    dist[i] = dist[min_prev] + matrix[min_prev][i];
                    path[i] = min_prev;
                }
            }
        }
        flag[min_prev] = true;
    }


    stack<int> tmp;
    vector<int> res;

    int e = dest;
    while (e != src) {
        if (e == -1) {
            Dijkstra new_dijkstra(INF, res);
            return new_dijkstra;
        }
        tmp.push(path[e]);
        e = path[e];
    }

    while (!tmp.empty()) {
        res.push_back(tmp.top());
        tmp.pop();
    }

    Dijkstra new_dijkstra(dist[dest], res);

    return new_dijkstra;
}

void perm(vector<int> list, int low, int high, vector<vector<int>>& c)
{
    if (low == high) {
        c.push_back(list);
    }
    else {
        for (int i = low; i <= high; i++) {
            swap(list[i], list[low]);
            perm(list, low + 1, high, c);
            swap(list[i], list[low]);
        }
    }
}

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
