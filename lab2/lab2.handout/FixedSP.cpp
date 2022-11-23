#include "FixedSP.h"
#include <stack>
#include <iostream>

vector<int> FixedSP::getFixedPointShortestPath(int source, vector<int> intermediates)
{
    vector<int> path;
    Dijkstra p(0, vector<int>(1, 0));
    vector<vector<Dijkstra>> min_distance(1000, vector<Dijkstra>(1000, p));
    for (int i = 0; i < intermediates.size(); i++) {
        min_distance[source][intermediates[i]] = getMinDistance(source, intermediates[i]);
    }
    for (int i = 0; i < intermediates.size(); i++) {
        for (int j = 0; j < intermediates.size(); j++) {
            if (i != j) {
                min_distance[intermediates[i]][intermediates[j]] = getMinDistance(intermediates[i], intermediates[j]);
            }
        }
    }
    for (int i = 0; i < intermediates.size(); i++) {
        min_distance[intermediates[i]][source] = getMinDistance(intermediates[i], source);
    }
    vector<vector<int>> c;
    perm(intermediates, 0, intermediates.size() - 1, c);

    int min = INT32_MAX;
    vector<int> min_c;
    for (auto cur : c) {
        /*for (int i = 0; i < cur.size(); i++) {
            cout << cur[i] << " ";
        }*/
        
        int tmp = min_distance[source][cur[0]].dist + min_distance[source][cur[cur.size() - 1]].dist;
        for (int i = 0; i < cur.size() - 1; i++) {
            tmp += min_distance[cur[i]][cur[i + 1]].dist;
        }
        if (tmp < min) {
            min = tmp;
            min_c = cur;
        }
    }
    //cout << min << endl;

    if (min >= INF)
        return path;

    //path.push_back(source);
    Dijkstra t = min_distance[source][min_c[0]];
    for (auto cur : t.path) {
        path.push_back(cur);
        //cout << cur << " ";
    }
    for (int i = 0; i < min_c.size() - 1; i++) {
        t = min_distance[min_c[i]][min_c[i + 1]];
        for (auto cur : t.path) {
            path.push_back(cur);
            //cout << cur << " ";
        }
    }
    t = min_distance[min_c[min_c.size()-1]][source];
    for (auto cur : t.path) {
        path.push_back(cur);
        //cout << cur << " ";
    }
    path.push_back(source);
    /*for (auto cur : path) {
        cout << cur << " ";
    }
    cout << endl;*/

    return path;
}

Dijkstra FixedSP::getMinDistance(int src, int dest)
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

    /*cout << src << "-" << dest << "最短路径长度为: " << dist[dest] << endl;
    cout << "路径为: " << endl;

    for (auto cur : path) {
        cout << cur << " ";
    }
    cout << dest << endl;*/

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

    //for (auto cur : res) {
    //    cout << cur << " ";
    //}
    //cout << dest << endl;

    Dijkstra new_dijkstra(dist[dest], res);

    return new_dijkstra;
}

void FixedSP::perm(vector<int> list, int low, int high, vector<vector<int>>& c)
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

void FixedSP::swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
