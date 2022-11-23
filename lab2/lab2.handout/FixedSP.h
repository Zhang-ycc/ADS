#include <vector>
#include <string>
#include <string.h>

using namespace std;

#define INF 1e8

struct Dijkstra
{
    int dist;
    vector<int> path;
    Dijkstra(int d, vector<int> p) {
        dist = d;
        path = p;
    }
};

class FixedSP
{
    vector<vector<int>> matrix;
    int n;
public:
    FixedSP(vector<vector<int>> m) {
        matrix = m;
        n = m.size();
    }
    ~FixedSP() {}

    vector<int> getFixedPointShortestPath(int source, vector<int> intermediates);

    Dijkstra getMinDistance(int src, int dest);

    void perm(vector<int> list, int low, int high, vector<vector<int>> &c);

    void swap(int& a, int& b);
};
