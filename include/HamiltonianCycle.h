//
// Created by redwan on 5/1/19.
//

#ifndef TASE_EXP_HAMILTONIANCYCLE_H
#define TASE_EXP_HAMILTONIANCYCLE_H

#include <iostream>
#include <vector>
#include <QtCore>
#include <QPointF>
#include <queue>
#include "TrajPlanner.h"
#include "MotionPrimitives.h"
using namespace std;
class HamiltonianCycle: public TrajPlanner{
public:
    HamiltonianCycle(int m, int n, float len, MotionPrimitives *move):row(m),col(n), len(len),TrajPlanner(move) {
        int N = m*n;
        vector<vector<bool>>M(N,vector<bool>(N,false));

        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < n; ++k) {
                int i = j*n + k;
                if(k>0)
                {
                    M[i-1][i] = M[i][i-1] = true;
                }
                if(j>0)
                {
                    M[i-n][i] = M[i][i-n] = true;
                }
            }
        }

        this->operator()(M);
    }
    void operator()(vector<vector<bool>>&graph){
        V = graph.size();
        hamCycle(graph);
    }

    bool generate(){
        trajectory_ = getCoord(len);
        return trajectory_.size()>0;
    }



    /* A utility function to check if the vertex v can be added at
   index 'pos' in the Hamiltonian Cycle constructed so far (stored
   in 'path[]') */
    bool isSafe(int v, vector<vector<bool>>&graph, vector<int>&path, int pos)
    {
        /* Check if this vertex is an adjacent vertex of the previously
           added vertex. */
        if (graph [ path[pos-1] ][ v ] == 0)
            return false;

        /* Check if the vertex has already been included.
          This step can be optimized by creating an array of size V */
        for (int i = 0; i < pos; i++)
            if (path[i] == v)
                return false;

        return true;
    }

    queue<QPointF> getCoord(int box_size){
        vector<QPointF> gridCoord;
        QString log="";
        for (int j = 0; j < row; ++j) {
            for (int k = 0; k < col; ++k) {
                double x = box_size*k+0.5*box_size;
                double y = box_size*j+0.5*box_size;
                QPointF point(x,y);
                gridCoord.push_back(point);
            }
        }
        queue<QPointF> traj;

        for(auto& i:indexes){
            auto point = gridCoord[i];

            double x = point.x();
            double y = point.y();
            traj.push(point);
            log +=QString::number(x) + ","+QString::number(y)+"\n";
        }

//        qDebug()<<gridCoord;

        return traj;
    }



    /* A recursive utility function to solve hamiltonian cycle problem */
    bool hamCycleUtil(vector<vector<bool>>&graph, vector<int>&path, int pos)
    {
        /* base case: If all vertices are included in Hamiltonian Cycle */
        if (pos == V)
        {
            // And if there is an edge from the last included vertex to the
            // first vertex
            if ( graph[ path[pos-1] ][ path[0] ] == 1 )
                return true;
            else
                return false;
        }

        // Try different vertices as a next candidate in Hamiltonian Cycle.
        // We don't try for 0 as we included 0 as starting point in in hamCycle()
        for (int v = 1; v < V; v++)
        {
            /* Check if this vertex can be added to Hamiltonian Cycle */
            if (isSafe(v, graph, path, pos))
            {
                path[pos] = v;

                /* recur to construct rest of the path */
                if (hamCycleUtil (graph, path, pos+1) == true)
                    return true;

                /* If adding vertex v doesn't lead to a solution,
                   then remove it */
                path[pos] = -1;
            }
        }

        /* If no vertex can be added to Hamiltonian Cycle constructed so far,
           then return false */
        return false;
    }
    bool hamCycle(vector<vector<bool>>&graph){

        /* Let us put vertex 0 as the first vertex in the path. If there is
           a Hamiltonian Cycle, then the path can be started from any point
           of the cycle as the graph is undirected */
        vector<int>path(V,-1);
        foundSolution = false;
        path[0] = 0;
        if ( hamCycleUtil(graph, path, 1) == false )
        {
            printf("\nSolution does not exist \n");
            return false;
        }

        foundSolution = true;
        indexes = path;
        for(auto &v:path)
            cout<<v<<"\t";
        return true;
    }

private:
    int V, row, col;
    float len;
    bool foundSolution;
    vector<int>indexes;

};


#endif //TASE_EXP_HAMILTONIANCYCLE_H
