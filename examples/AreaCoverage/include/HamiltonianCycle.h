//
// Created by redwan on 6/2/19.
//

#ifndef TASE_EXP_HAMILTONIANCYCLE_H
#define TASE_EXP_HAMILTONIANCYCLE_H

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
typedef vector<vector<bool>> Adj;
struct Point{
    int x, y;

    friend ostream &operator<<(ostream &os, const Point &point)
    {
        os << "(x: " << point.x << " y: " << point.y <<")";
        return os;
    }
};

typedef vector<Point>PATH;

class HamiltonianCycle {
public:
    HamiltonianCycle();
    Adj gridAdjacency(int m, int n);
    bool hamCycle(Adj &graph);
    PATH getTrajectory();


private:
    int V, row, col;
    bool foundSolution;
    vector<int>indexes;
protected:
    bool hamCycleUtil(Adj &graph, vector<int>&path, int pos);
    bool isSafe(int v, vector<vector<bool>>&graph, vector<int>&path, int pos);
};


#endif //TASE_EXP_HAMILTONIANCYCLE_H
